"""
Login screen for the AUC Library System
"""

import customtkinter as ctk
import sys
import os
from styles import COLORS, FONTS, SPACING, SIZES
from widgets import StyledButton, StyledEntry, StyledLabel, Card, NotificationBanner

# Add parent directory to path to import the C++ library
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

try:
    import library_system
    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False
    print("Warning: library_system module not found. Using mock data for development.")


class LoginScreen(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color=COLORS["bg_dark"])
        self.controller = controller
        
        # Use shared library system from controller
        self.lib_system = self.controller.lib_system
        
        # Center container
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)
        
        # Main card
        main_card = Card(self, width=500)
        main_card.grid(row=0, column=0, padx=SPACING["xl"], pady=SPACING["xl"])
        main_card.grid_propagate(False)
        
        # Title section
        title_frame = ctk.CTkFrame(main_card, fg_color="transparent")
        title_frame.pack(pady=(SPACING["xl"], SPACING["lg"]), padx=SPACING["lg"])
        
        StyledLabel(
            title_frame,
            text="🎓 AUC Library",
            size="title",
            text_color=COLORS["primary"]
        ).pack()
        
        StyledLabel(
            title_frame,
            text="Resource Management System",
            size="body",
            text_color=COLORS["text_secondary"]
        ).pack()
        
        # Separator
        separator = ctk.CTkFrame(main_card, height=3, fg_color=COLORS["primary"])
        separator.pack(fill="x", padx=SPACING["lg"], pady=SPACING["md"])
        
        # Login form
        form_frame = ctk.CTkFrame(main_card, fg_color="transparent")
        form_frame.pack(pady=SPACING["md"], padx=SPACING["lg"], fill="both", expand=True)
        
        # Username
        StyledLabel(form_frame, text="Username", size="body_large").pack(anchor="w", pady=(SPACING["md"], 5))
        self.username_entry = StyledEntry(form_frame, placeholder="Enter your username")
        self.username_entry.pack(fill="x", pady=(0, SPACING["md"]))
        
        # Password
        StyledLabel(form_frame, text="Password", size="body_large").pack(anchor="w", pady=(0, 5))
        self.password_entry = StyledEntry(form_frame, placeholder="Enter your password", show="•")
        self.password_entry.pack(fill="x", pady=(0, SPACING["md"]))
        
        # Bind Enter key to login
        self.password_entry.bind("<Return>", lambda e: self.handle_login())
        self.username_entry.bind("<Return>", lambda e: self.password_entry.focus())
        
        # Error message placeholder
        self.error_frame = ctk.CTkFrame(form_frame, fg_color="transparent", height=40)
        self.error_frame.pack(fill="x", pady=(0, SPACING["sm"]))
        self.error_banner = None
        
        # Login button
        StyledButton(
            form_frame,
            text="Login",
            variant="primary",
            command=self.handle_login
        ).pack(fill="x", pady=(SPACING["sm"], SPACING["md"]))
        
        # Info section
        info_frame = ctk.CTkFrame(main_card, fg_color=COLORS["bg_light"], corner_radius=SIZES["button_corner"])
        info_frame.pack(fill="x", padx=SPACING["lg"], pady=(0, SPACING["lg"]))
        
        StyledLabel(
            info_frame,
            text="ℹ️  Test Accounts",
            size="small",
            text_color=COLORS["accent_blue"]
        ).pack(pady=(SPACING["sm"], 5), padx=SPACING["sm"])
        
        StyledLabel(
            info_frame,
            text="User: user1 / pass: pass1\nAdmin: admin / pass: admin123",
            size="tiny",
            text_color=COLORS["text_secondary"],
            justify="left"
        ).pack(pady=(0, SPACING["sm"]), padx=SPACING["sm"])
    
    def show_error(self, message):
        """Display error message"""
        if self.error_banner:
            self.error_banner.destroy()
        
        self.error_banner = NotificationBanner(
            self.error_frame,
            message=f"❌ {message}",
            message_type="error"
        )
        self.error_banner.pack(fill="x")
    
    def hide_error(self):
        """Hide error message"""
        if self.error_banner:
            self.error_banner.destroy()
            self.error_banner = None
    
    def handle_login(self):
        """Handle login button click"""
        username = self.username_entry.get().strip()
        password = self.password_entry.get().strip()
        
        # Validation
        if not username or not password:
            self.show_error("Please enter both username and password")
            return
        
        # Attempt login
        if self.lib_system and LIBRARY_AVAILABLE:
            result = self.lib_system.login(username, password)
            if result["success"]:
                self.hide_error()
                self.controller.set_user(result["username"], result["is_admin"])
                # Clear fields
                self.username_entry.delete(0, "end")
                self.password_entry.delete(0, "end")
            else:
                self.show_error("Invalid username or password")
        else:
            # Mock login for development
            if username in ["admin", "user1"]:
                is_admin = (username == "admin")
                self.hide_error()
                self.controller.set_user(username, is_admin)
                self.username_entry.delete(0, "end")
                self.password_entry.delete(0, "end")
            else:
                self.show_error("Invalid username or password")
