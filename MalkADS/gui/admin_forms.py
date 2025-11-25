"""
Admin management forms for adding/removing resources
"""

import customtkinter as ctk
import sys
import os
from styles import COLORS, FONTS, SPACING, SIZES
from widgets import (
    StyledButton,
    StyledEntry,
    StyledLabel,
    Card,
    NotificationBanner,
    ScrollableFrame,
)

# Add parent directory to path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

try:
    import library_system

    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False


class ManagementDialog(ctk.CTkToplevel):
    """Dialog for admin resource management"""

    def __init__(self, parent, action_type, lib_system=None):
        super().__init__(parent)
        self.action_type = action_type

        # Use shared library system instance
        self.lib_system = lib_system
        if self.lib_system is None and LIBRARY_AVAILABLE:
            # Fallback if not passed (shouldn't happen in normal flow)
            self.lib_system = library_system.LibrarySystem()

        # Parse action
        parts = action_type.split("_")
        self.action = parts[0]  # add or remove
        self.resource_type = parts[1]  # book, laptop, or room

        # Window configuration
        title = f"{self.action.title()} {self.resource_type.title()}"
        self.title(title)
        self.geometry("550x500")
        self.resizable(True, True)  # Enable resizing

        # Center window
        self.update_idletasks()
        x = (self.winfo_screenwidth() // 2) - (550 // 2)
        y = (self.winfo_screenheight() // 2) - (500 // 2)
        self.geometry(f"550x500+{x}+{y}")

        # Main container
        main_frame = Card(self, fg_color=COLORS["bg_dark"])
        main_frame.pack(
            expand=True, fill="both", padx=SPACING["lg"], pady=SPACING["lg"]
        )

        # Header
        color = COLORS["success"] if self.action == "add" else COLORS["error"]
        header = ctk.CTkFrame(
            main_frame, fg_color=color, corner_radius=SIZES["button_corner"]
        )
        header.pack(fill="x", pady=(0, SPACING["lg"]))

        icon = {"book": "📚", "laptop": "💻", "room": "🏛️"}[self.resource_type]
        StyledLabel(
            header,
            text=f"{icon} {title}",
            size="heading",
            text_color=COLORS["text_primary"],
        ).pack(pady=SPACING["md"])

        # Form section
        form_frame = ScrollableFrame(main_frame, fg_color="transparent")
        form_frame.pack(fill="both", expand=True, padx=SPACING["md"])

        if self.action == "add":
            self.create_add_form(form_frame)
        else:
            self.create_remove_form(form_frame)

        # Notification area
        self.notification_frame = ctk.CTkFrame(
            form_frame, fg_color="transparent", height=50
        )
        self.notification_frame.pack(fill="x", pady=(SPACING["md"], 0))
        self.notification_banner = None

        # Buttons
        button_frame = ctk.CTkFrame(form_frame, fg_color="transparent")
        button_frame.pack(fill="x", pady=(SPACING["lg"], 0))

        StyledButton(
            button_frame,
            text="Cancel",
            variant="error",
            width=150,
            command=self.destroy,
        ).pack(side="left", padx=(0, SPACING["sm"]))

        button_variant = "success" if self.action == "add" else "error"
        button_text = "Add" if self.action == "add" else "Remove"

        StyledButton(
            button_frame,
            text=button_text,
            variant=button_variant,
            width=150,
            command=self.handle_action,
        ).pack(side="right")

    def create_add_form(self, parent):
        """Create form for adding resources"""
        if self.resource_type == "book":
            # Book ID
            StyledLabel(parent, text="Book ID:", size="body_large").pack(
                anchor="w", pady=(SPACING["md"], 5)
            )
            self.id_entry = StyledEntry(parent, placeholder="e.g., B0001")
            self.id_entry.pack(fill="x", pady=(0, SPACING["md"]))

            # Title
            StyledLabel(parent, text="Title:", size="body_large").pack(
                anchor="w", pady=(0, 5)
            )
            self.title_entry = StyledEntry(parent, placeholder="Book title")
            self.title_entry.pack(fill="x", pady=(0, SPACING["md"]))

            # Author
            StyledLabel(parent, text="Author:", size="body_large").pack(
                anchor="w", pady=(0, 5)
            )
            self.author_entry = StyledEntry(parent, placeholder="Author name")
            self.author_entry.pack(fill="x", pady=(0, SPACING["md"]))
        else:
            # For laptops and rooms, just need ID
            resource_name = self.resource_type.title()
            StyledLabel(parent, text=f"{resource_name} ID:", size="body_large").pack(
                anchor="w", pady=(SPACING["md"], 5)
            )

            placeholder = (
                "e.g., L001" if self.resource_type == "laptop" else "e.g., R101"
            )
            self.id_entry = StyledEntry(parent, placeholder=placeholder)
            self.id_entry.pack(fill="x", pady=(0, SPACING["md"]))

        # Info box
        info_box = ctk.CTkFrame(
            parent, fg_color=COLORS["bg_light"], corner_radius=SIZES["button_corner"]
        )
        info_box.pack(fill="x", pady=(SPACING["md"], 0))

        info_text = "New resource will be added to the library system"
        StyledLabel(
            info_box,
            text=f"ℹ️  {info_text}",
            size="small",
            text_color=COLORS["text_secondary"],
        ).pack(pady=SPACING["sm"], padx=SPACING["sm"])

    def create_remove_form(self, parent):
        """Create form for removing resources"""
        StyledLabel(
            parent,
            text=f"Select {self.resource_type.title()} to Remove:",
            size="body_large",
        ).pack(anchor="w", pady=(SPACING["md"], 5))

        self.resource_var = ctk.StringVar()
        self.resource_dropdown = ctk.CTkComboBox(
            parent,
            variable=self.resource_var,
            values=self.get_resources(),
            font=FONTS["body"],
            height=SIZES["input_height"],
            state="readonly",
        )
        self.resource_dropdown.pack(fill="x", pady=(0, SPACING["lg"]))

        # Warning box
        warning_box = ctk.CTkFrame(
            parent, fg_color=COLORS["warning"], corner_radius=SIZES["button_corner"]
        )
        warning_box.pack(fill="x", pady=(SPACING["md"], 0))

        StyledLabel(
            warning_box,
            text="⚠️  Warning: This action cannot be undone!",
            size="small",
            text_color=COLORS["text_primary"],
        ).pack(pady=SPACING["sm"], padx=SPACING["sm"])

    def get_resources(self):
        """Get existing resources"""
        if self.lib_system and LIBRARY_AVAILABLE:
            try:
                if self.resource_type == "room":
                    resources = self.lib_system.get_rooms()
                elif self.resource_type == "laptop":
                    resources = self.lib_system.get_laptops()
                else:
                    resources = self.lib_system.get_books()
                return list(resources) if resources else ["No resources available"]
            except Exception as e:
                print(f"Error getting resources: {e}")
                return ["Error loading resources"]
        else:
            # Mock data
            if self.resource_type == "room":
                return ["R101", "R102", "R103"]
            elif self.resource_type == "laptop":
                return ["L001", "L002"]
            else:
                return ["B0001", "B0002"]

    def show_notification(self, message, message_type="info"):
        """Show notification message"""
        if self.notification_banner:
            self.notification_banner.destroy()

        icon = {"success": "✅", "error": "❌", "warning": "⚠️", "info": "ℹ️"}[
            message_type
        ]
        self.notification_banner = NotificationBanner(
            self.notification_frame,
            message=f"{icon} {message}",
            message_type=message_type,
        )
        self.notification_banner.pack(fill="x")

    def handle_action(self):
        """Handle add or remove action"""
        if self.action == "add":
            self.handle_add()
        else:
            self.handle_remove()

    def handle_add(self):
        """Handle adding a resource"""
        resource_id = self.id_entry.get().strip()

        if not resource_id:
            self.show_notification("Please enter a resource ID", "error")
            return

        # For books, get additional fields
        if self.resource_type == "book":
            title = self.title_entry.get().strip()
            author = self.author_entry.get().strip()

            if not title or not author:
                self.show_notification("Please fill in all fields", "error")
                return

        # Call appropriate library method
        if self.lib_system and LIBRARY_AVAILABLE:
            try:
                if self.resource_type == "room":
                    self.lib_system.add_room(resource_id)
                elif self.resource_type == "laptop":
                    self.lib_system.add_laptop(resource_id)
                else:
                    self.lib_system.add_book(resource_id, title, author)

                self.show_notification(
                    f"{self.resource_type.title()} added successfully!", "success"
                )
                self.after(1500, self.destroy)
            except Exception as e:
                self.show_notification(f"Error: {str(e)}", "error")
        else:
            self.show_notification(
                f"{self.resource_type.title()} added successfully!", "success"
            )
            self.after(1500, self.destroy)

    def handle_remove(self):
        """Handle removing a resource"""
        resource_id = self.resource_var.get()

        if not resource_id or resource_id == "No resources available":
            self.show_notification("Please select a resource", "error")
            return

        # Call appropriate library method
        if self.lib_system and LIBRARY_AVAILABLE:
            try:
                if self.resource_type == "room":
                    self.lib_system.remove_room(resource_id)
                elif self.resource_type == "laptop":
                    self.lib_system.remove_laptop(resource_id)
                else:
                    self.lib_system.remove_book(resource_id)

                self.show_notification(
                    f"{self.resource_type.title()} removed successfully!", "success"
                )
                self.after(1500, self.destroy)
            except Exception as e:
                self.show_notification(f"Error: {str(e)}", "error")
        else:
            self.show_notification(
                f"{self.resource_type.title()} removed successfully!", "success"
            )
            self.after(1500, self.destroy)
