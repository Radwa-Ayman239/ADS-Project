"""
Booking forms for rooms, laptops, and books
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
    TimeSlotEntry,
    ScrollableFrame,
)
from book_search import BookSearchDialog
from time_slot_grid import TimeSlotGrid

# Add parent directory to path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

try:
    import library_system

    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False


class BookingDialog(ctk.CTkToplevel):
    """Dialog for making bookings"""

    def __init__(self, parent, booking_type, username, lib_system=None):
        super().__init__(parent)
        self.booking_type = booking_type
        self.username = username
        self.parent = parent

        # Use shared library system instance
        self.lib_system = lib_system

        # Window configuration
        self.title(f"Book {booking_type.title()}")

        # Larger window for books due to search interface
        if booking_type == "book":
            self.geometry("600x750")
        else:
            self.geometry("600x700")

        self.resizable(True, True)

        # Center window
        self.update_idletasks()
        x = (self.winfo_screenwidth() // 2) - (600 // 2)
        y = (self.winfo_screenheight() // 2) - (700 // 2)
        self.geometry(f"600x700+{x}+{y}")

        # Main container
        main_frame = Card(self, fg_color=COLORS["bg_dark"])
        main_frame.pack(
            expand=True, fill="both", padx=SPACING["lg"], pady=SPACING["lg"]
        )

        # Header
        header = ctk.CTkFrame(
            main_frame, fg_color=COLORS["primary"], corner_radius=SIZES["button_corner"]
        )
        header.pack(fill="x", pady=(0, SPACING["lg"]))

        icon = {"room": "🏛️", "laptop": "💻", "book": "📚"}[booking_type]
        StyledLabel(
            header,
            text=f"{icon} Book {booking_type.title()}",
            size="heading",
            text_color=COLORS["text_primary"],
        ).pack(pady=SPACING["md"])

        # Form section
        form_frame = ScrollableFrame(main_frame, fg_color="transparent")
        form_frame.pack(fill="both", expand=True, padx=SPACING["md"])

        # Resource selection
        StyledLabel(
            form_frame, text=f"Select {booking_type.title()}:", size="body_large"
        ).pack(anchor="w", pady=(SPACING["md"], 5))

        # For books, use search dialog instead of dropdown
        if booking_type == "book":
            # Selected book display
            self.selected_book_frame = Card(form_frame, fg_color=COLORS["bg_light"])
            self.selected_book_frame.pack(fill="x", pady=(0, SPACING["sm"]))

            self.selected_book_label = StyledLabel(
                self.selected_book_frame,
                text="No book selected - click 'Search Books' below",
                size="body",
                text_color=COLORS["text_secondary"],
            )
            self.selected_book_label.pack(pady=SPACING["sm"], padx=SPACING["sm"])

            self.resource_var = ctk.StringVar()

            # Search button
            StyledButton(
                form_frame,
                text="📚 Search Books",
                variant="primary",
                width=200,
                command=self.open_book_search,
            ).pack(pady=(0, SPACING["lg"]))

        else:
            # Dropdown for rooms (laptops are auto-assigned)
            resources = []
            if self.lib_system:
                if booking_type == "room":
                    resources = self.lib_system.get_rooms()
                # Laptop resources not needed for dropdown as they are auto-assigned

            if booking_type == "laptop":
                StyledLabel(
                    form_frame,
                    text="Laptop will be automatically assigned based on availability.",
                    text_color=COLORS["text_secondary"],
                    size="body",
                ).pack(anchor="w", pady=(0, SPACING["lg"]))
                self.resource_var = ctk.StringVar(value="AUTO")
            else:
                self.resource_var = ctk.StringVar(
                    value=resources[0] if resources else ""
                )

                self.resource_dropdown = ctk.CTkOptionMenu(
                    form_frame,
                    variable=self.resource_var,
                    values=resources if resources else ["No resources"],
                    fg_color=COLORS["bg_card"],
                    button_color=COLORS["primary"],
                    button_hover_color=COLORS["primary_hover"],
                    text_color=COLORS["text_primary"],
                    font=FONTS["body"],
                    height=SIZES["input_height"],
                    command=self.on_resource_change,
                )
                self.resource_dropdown.pack(fill="x", pady=(0, SPACING["lg"]))

                if not resources:
                    StyledLabel(
                        form_frame,
                        text="No resources found!",
                        text_color=COLORS["error"],
                        size="small",
                    ).pack(anchor="w")

        # Time selection
        if booking_type == "room":
            StyledLabel(form_frame, text="Select Time Slot:", size="body_large").pack(
                anchor="w", pady=(0, 5)
            )

            self.time_slots = TimeSlotGrid(form_frame)
            self.time_slots.pack(fill="x", pady=(0, SPACING["lg"]))

            # Trigger initial load if resources exist
            if resources:
                self.on_resource_change(resources[0])

        else:
            StyledLabel(form_frame, text="Booking Time:", size="body_large").pack(
                anchor="w", pady=(0, 5)
            )

            self.time_slots = TimeSlotEntry(form_frame)
            self.time_slots.pack(fill="x", pady=(0, SPACING["lg"]))

        # Notification area
        self.notification_frame = ctk.CTkFrame(
            form_frame, fg_color="transparent", height=50
        )
        self.notification_frame.pack(fill="x", pady=(0, SPACING["md"]))
        self.notification_banner = None

        # Action buttons
        btn_frame = ctk.CTkFrame(main_frame, fg_color="transparent")
        btn_frame.pack(fill="x", pady=SPACING["md"])

        StyledButton(
            btn_frame,
            text="Cancel",
            variant="secondary",
            width=100,
            command=self.destroy,
        ).pack(side="left", padx=(0, SPACING["md"]))

        StyledButton(
            btn_frame,
            text="Confirm Booking",
            variant="success",
            width=100,
            command=self.confirm_booking,
        ).pack(side="right")

    def on_resource_change(self, choice):
        """Handle resource selection change"""
        if (
            self.booking_type == "room"
            and hasattr(self, "time_slots")
            and self.lib_system
        ):
            # Fetch bookings for selected room
            try:
                bookings = self.lib_system.get_room_bookings(choice)
                self.time_slots.set_booked_slots(bookings)
            except Exception as e:
                print(f"Error fetching bookings: {e}")

    def open_book_search(self):
        """Open the book search dialog"""
        dialog = BookSearchDialog(self, self.lib_system, self.on_book_selected)
        dialog.grab_set()

    def on_book_selected(self, book_id, title, author):
        """Callback when a book is selected"""
        self.resource_var.set(book_id)
        self.selected_book_label.configure(
            text=f"Selected: {title}\nby {author}", text_color=COLORS["text_primary"]
        )

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

    def confirm_booking(self):
        resource_id = self.resource_var.get()
        if not resource_id or resource_id == "No resources":
            self.show_notification("Please select a resource", "error")
            return

        if self.booking_type == "room":
            start = self.time_slots.selected_start
            end = self.time_slots.selected_end
            if start is None or end is None:
                self.show_notification("Please select a time slot", "error")
                return
        else:
            start, end = self.time_slots.get_times()
            if start is None or end is None:
                self.show_notification(
                    "Please enter valid start and end times", "error"
                )
                return

        if start >= end:
            self.show_notification("End time must be after start time", "error")
            return

        # Attempt booking
        success = False
        message = ""

        try:
            if self.lib_system:
                if self.booking_type == "room":
                    result = self.lib_system.book_room(
                        resource_id, start, end, self.username
                    )
                elif self.booking_type == "laptop":
                    # Use auto-assignment for laptops
                    result = self.lib_system.borrow_any_laptop(
                        start, end, self.username
                    )
                elif self.booking_type == "book":
                    result = self.lib_system.borrow_book(
                        resource_id, start, end, self.username
                    )

                if result["success"]:
                    self.show_notification(result["message"], "success")
                    if hasattr(self.parent, "refresh_bookings"):
                        self.parent.refresh_bookings()
                    self.after(2000, self.destroy)  # Wait a bit longer to read message
                else:
                    self.show_notification(result["message"], "error")
            else:
                self.show_notification("Library system not connected", "error")

        except Exception as e:
            self.show_notification(f"Error: {str(e)}", "error")
