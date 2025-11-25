"""
AUC Library System - Modern GUI Application
Main entry point for the library management system
"""

import customtkinter as ctk
import sys
import os
from login_screen import LoginScreen
from user_dashboard import UserDashboard
from admin_dashboard import AdminDashboard

# Add parent directory to path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

try:
    import library_system

    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False

# Set appearance and color theme
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")


class LibraryApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        # Configure window
        self.title("AUC Library System")
        self.geometry("900x700")

        # Center window
        self.update_idletasks()
        x = (self.winfo_screenwidth() // 2) - (900 // 2)
        y = (self.winfo_screenheight() // 2) - (700 // 2)
        self.geometry(f"900x700+{x}+{y}")

        # Initialize shared library system instance
        if LIBRARY_AVAILABLE:
            self.lib_system = library_system.LibrarySystem()
        else:
            self.lib_system = None

        # State
        self.current_user = None
        self.is_admin = False
        self.current_screen = None

        # Container for screens
        self.container = ctk.CTkFrame(self, fg_color="transparent")
        self.container.pack(fill="both", expand=True)

        # Dictionary to hold all screens
        self.frames = {}

        # Initialize screens
        self.create_screens()

        # Show login screen
        self.show_screen("login")

        # Handle window closing
        self.protocol("WM_DELETE_WINDOW", self.on_closing)

    def on_closing(self):
        """Handle application closing"""
        if self.lib_system:
            print("Saving data before exit...")
            self.lib_system.save()
        self.destroy()

    def create_screens(self):
        """Initialize all application screens"""
        # Login screen - pass lib_system
        self.frames["login"] = LoginScreen(self.container, self)

        # User dashboard
        self.frames["user_dashboard"] = UserDashboard(self.container, self)

        # Admin dashboard
        self.frames["admin_dashboard"] = AdminDashboard(self.container, self)

        # Place all frames in the same location
        for frame in self.frames.values():
            frame.grid(row=0, column=0, sticky="nsew")

        self.container.grid_rowconfigure(0, weight=1)
        self.container.grid_columnconfigure(0, weight=1)

    def show_screen(self, screen_name):
        """Show the specified screen"""
        frame = self.frames[screen_name]
        frame.tkraise()

        # Refresh the screen if it has a refresh method
        if hasattr(frame, "refresh"):
            frame.refresh()

    def set_user(self, username, is_admin):
        """Set the current logged-in user"""
        self.current_user = username
        self.is_admin = is_admin

        # Navigate to appropriate dashboard
        if is_admin:
            self.show_screen("admin_dashboard")
        else:
            self.show_screen("user_dashboard")

    def logout(self):
        """Log out the current user"""
        self.current_user = None
        self.is_admin = False
        # Reload library system to get fresh data
        if LIBRARY_AVAILABLE:
            self.lib_system = library_system.LibrarySystem()
        self.show_screen("login")

    def toggle_theme(self):
        """Toggle between Light and Dark mode"""
        if ctk.get_appearance_mode() == "Dark":
            ctk.set_appearance_mode("Light")
        else:
            ctk.set_appearance_mode("Dark")


if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
