"""
Login screen for the AUC Library System
"""

import customtkinter as ctk
import sys
import os
from styles import COLORS, FONTS, SPACING, SIZES, ANIMATIONS
from widgets import (
    AnimatedButton,
    StyledEntry,
    StyledLabel,
    ModernCard,
    NotificationBanner,
)

# Add parent directory to path to import the C++ library
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

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

        # Main card with animation
        self.main_card = ModernCard(self, width=500, fg_color=COLORS["bg_card"])
        self.main_card.grid(row=0, column=0, padx=SPACING["xl"], pady=SPACING["xl"])
        self.main_card.grid_propagate(False)

        # Initial state for animation
        self.main_card.place(relx=0.5, rely=0.6, anchor="center")
        self.main_card.configure(fg_color="transparent")  # Hide initially

        # Title section
        title_frame = ctk.CTkFrame(self.main_card, fg_color="transparent")
        title_frame.pack(pady=(SPACING["xl"], SPACING["lg"]), padx=SPACING["lg"])

        StyledLabel(
            title_frame,
            text="🎓 AUC Library",
            size="title",
            text_color=COLORS["primary"],
        ).pack()

        StyledLabel(
            title_frame,
            text="Resource Management System",
            size="body",
            text_color=COLORS["text_secondary"],
        ).pack()

        # Separator with gradient effect (simulated)
        separator = ctk.CTkFrame(self.main_card, height=3, fg_color=COLORS["primary"])
        separator.pack(fill="x", padx=SPACING["lg"], pady=SPACING["md"])

        # Login form
        form_frame = ctk.CTkFrame(self.main_card, fg_color="transparent")
        form_frame.pack(
            pady=SPACING["md"], padx=SPACING["lg"], fill="both", expand=True
        )

        # Username
        StyledLabel(form_frame, text="Username", size="body_large").pack(
            anchor="w", pady=(SPACING["md"], 5)
        )
        self.username_entry = StyledEntry(form_frame, placeholder="Enter your username")
        self.username_entry.pack(fill="x", pady=(0, SPACING["md"]))

        # Password
        StyledLabel(form_frame, text="Password", size="body_large").pack(
            anchor="w", pady=(0, 5)
        )
        self.password_entry = StyledEntry(
            form_frame, placeholder="Enter your password", show="•"
        )
        self.password_entry.pack(fill="x", pady=(0, SPACING["md"]))

        # Password visibility toggle
        self.show_password = False
        self.toggle_btn = ctk.CTkButton(
            self.password_entry,
            text="👁️",
            width=30,
            height=30,
            fg_color="transparent",
            hover_color=COLORS["bg_medium"],
            text_color=COLORS["text_secondary"],
            command=self.toggle_password_visibility,
        )
        self.toggle_btn.place(relx=0.95, rely=0.5, anchor="e")

        # Bind Enter key to login
        self.password_entry.bind("<Return>", lambda e: self.handle_login())
        self.username_entry.bind("<Return>", lambda e: self.password_entry.focus())

        # Error message placeholder
        self.error_frame = ctk.CTkFrame(form_frame, fg_color="transparent", height=40)
        self.error_frame.pack(fill="x", pady=(0, SPACING["sm"]))
        self.error_banner = None

        # Login button
        AnimatedButton(
            form_frame, text="Login", variant="primary", command=self.handle_login
        ).pack(fill="x", pady=(SPACING["sm"], SPACING["md"]))

        # Info section
        info_frame = ctk.CTkFrame(
            self.main_card,
            fg_color=COLORS["bg_medium"],
            corner_radius=SIZES["button_corner"],
        )
        info_frame.pack(fill="x", padx=SPACING["lg"], pady=(0, SPACING["lg"]))

        StyledLabel(
            info_frame,
            text="ℹ️  Test Accounts",
            size="small",
            text_color=COLORS["accent_cyan"],
        ).pack(pady=(SPACING["sm"], 5), padx=SPACING["sm"])

        StyledLabel(
            info_frame,
            text="User: user1 / pass: pass1\nAdmin: admin / pass: admin123",
            size="tiny",
            text_color=COLORS["text_secondary"],
            justify="left",
        ).pack(pady=(0, SPACING["sm"]), padx=SPACING["sm"])

        # Theme toggle
        self.theme_toggle = ctk.CTkButton(
            self,
            text="🌗",
            width=40,
            height=40,
            fg_color="transparent",
            text_color=COLORS["text_secondary"],
            font=("Roboto", 20),
            hover_color=COLORS["bg_medium"],
            command=self.controller.toggle_theme,
        )
        self.theme_toggle.place(relx=0.95, rely=0.05, anchor="ne")

        # Trigger entrance animation
        self.after(100, self.animate_entrance)

    def animate_entrance(self):
        """Animate the login card sliding up"""
        start_y = 0.6
        end_y = 0.5
        steps = 20
        duration = ANIMATIONS["medium"]
        step_time = duration // steps
        y_step = (start_y - end_y) / steps

        self.main_card.configure(fg_color=COLORS["bg_card"])  # Show card

        def step(current_step):
            if current_step < steps:
                new_y = start_y - (y_step * current_step)
                self.main_card.place(relx=0.5, rely=new_y, anchor="center")
                self.after(step_time, lambda: step(current_step + 1))
            else:
                self.main_card.place(relx=0.5, rely=end_y, anchor="center")

        step(0)

    def toggle_password_visibility(self):
        """Toggle password visibility"""
        self.show_password = not self.show_password
        if self.show_password:
            self.password_entry.configure(show="")
            self.toggle_btn.configure(text="🔒")  # Icon for hiding
        else:
            self.password_entry.configure(show="•")
            self.toggle_btn.configure(text="👁️")  # Icon for showing

    def show_error(self, message):
        """Display error message"""
        if self.error_banner:
            self.error_banner.destroy()

        self.error_banner = NotificationBanner(
            self.error_frame, message=f"❌ {message}", message_type="error"
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
                is_admin = username == "admin"
                self.hide_error()
                self.controller.set_user(username, is_admin)
                self.username_entry.delete(0, "end")
                self.password_entry.delete(0, "end")
            else:
                self.show_error("Invalid username or password")
