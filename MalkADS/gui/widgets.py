"""
Custom reusable widgets with consistent styling and animations
"""

import customtkinter as ctk
import datetime
from styles import COLORS, FONTS, SIZES, SPACING, ANIMATIONS


class AnimatedButton(ctk.CTkButton):
    """Button with hover animation effects"""

    def __init__(self, master, text="Button", variant="primary", **kwargs):
        # Set default styling based on variant
        if variant == "primary":
            self.default_color = COLORS["primary"]
            self.hover_color = COLORS["primary_hover"]
        elif variant == "secondary":
            self.default_color = COLORS["secondary"]
            self.hover_color = COLORS["secondary_hover"]
        elif variant == "success":
            self.default_color = COLORS["success"]
            self.hover_color = COLORS["success_hover"]
        elif variant == "error":
            self.default_color = COLORS["error"]
            self.hover_color = COLORS["error_hover"]
        elif variant == "warning":
            self.default_color = COLORS["warning"]
            self.hover_color = COLORS["warning_hover"]
        else:
            self.default_color = COLORS["bg_light"]
            self.hover_color = COLORS["bg_medium"]

        super().__init__(
            master,
            text=text,
            fg_color=self.default_color,
            hover_color=self.hover_color,
            font=FONTS["button"],
            height=SIZES["button_height"],
            corner_radius=SIZES["button_corner"],
            **kwargs
        )

        # Bind events for animation
        self.bind("<Enter>", self.on_enter)
        self.bind("<Leave>", self.on_leave)

    def on_enter(self, event):
        """Handle mouse enter"""
        self.configure(border_width=2, border_color=COLORS["text_secondary"])

    def on_leave(self, event):
        """Handle mouse leave"""
        self.configure(border_width=0)


class StyledEntry(ctk.CTkEntry):
    """Custom entry field with consistent styling"""

    def __init__(self, master, placeholder="", **kwargs):
        super().__init__(
            master,
            placeholder_text=placeholder,
            font=FONTS["body"],
            height=SIZES["input_height"],
            corner_radius=SIZES["input_corner"],
            border_width=2,
            fg_color=COLORS["bg_medium"],
            border_color=COLORS["bg_light"],
            placeholder_text_color=COLORS["text_dim"],
            text_color=COLORS["text_primary"],
            **kwargs
        )

        self.bind("<FocusIn>", self.on_focus_in)
        self.bind("<FocusOut>", self.on_focus_out)

    def on_focus_in(self, event):
        self.configure(border_color=COLORS["primary"])

    def on_focus_out(self, event):
        self.configure(border_color=COLORS["bg_light"])


class StyledLabel(ctk.CTkLabel):
    """Custom label with consistent styling"""

    def __init__(self, master, text="", size="body", **kwargs):
        text_color = kwargs.pop("text_color", COLORS["text_primary"])
        super().__init__(
            master,
            text=text,
            font=FONTS.get(size, FONTS["body"]),
            text_color=text_color,
            **kwargs
        )


class ModernCard(ctk.CTkFrame):
    """Card container with shadow effect and hover glow"""

    def __init__(self, master, **kwargs):
        # Set default fg_color if not provided
        if "fg_color" not in kwargs:
            kwargs["fg_color"] = COLORS["bg_card"]
        if "corner_radius" not in kwargs:
            kwargs["corner_radius"] = SIZES["card_corner"]

        super().__init__(master, **kwargs)

        # Add subtle border
        self.configure(border_width=1, border_color=COLORS["bg_light"])

        self.bind("<Enter>", self.on_enter)
        self.bind("<Leave>", self.on_leave)

    def on_enter(self, event):
        self.configure(border_color=COLORS["primary"], border_width=2)

    def on_leave(self, event):
        self.configure(border_color=COLORS["bg_light"], border_width=1)


class NotificationBanner(ctk.CTkFrame):
    """Notification banner for success/error messages"""

    def __init__(self, master, message="", message_type="info", **kwargs):
        if message_type == "success":
            fg_color = COLORS["success"]
        elif message_type == "error":
            fg_color = COLORS["error"]
        elif message_type == "warning":
            fg_color = COLORS["warning"]
        else:
            fg_color = COLORS["primary"]

        super().__init__(
            master, fg_color=fg_color, corner_radius=SIZES["button_corner"], **kwargs
        )

        self.label = StyledLabel(self, text=message, text_color=COLORS["text_primary"])
        self.label.pack(padx=SPACING["md"], pady=SPACING["sm"])

    def update_message(self, message, message_type="info"):
        """Update the notification message"""
        if message_type == "success":
            self.configure(fg_color=COLORS["success"])
        elif message_type == "error":
            self.configure(fg_color=COLORS["error"])
        elif message_type == "warning":
            self.configure(fg_color=COLORS["warning"])
        else:
            self.configure(fg_color=COLORS["primary"])

        self.label.configure(text=message)


class ScrollableFrame(ctk.CTkScrollableFrame):
    """Scrollable frame with consistent styling"""

    def __init__(self, master, **kwargs):
        # Set default fg_color if not provided
        if "fg_color" not in kwargs:
            kwargs["fg_color"] = COLORS["bg_card"]

        super().__init__(
            master,
            corner_radius=SIZES["card_corner"],
            scrollbar_button_color=COLORS["primary"],
            scrollbar_button_hover_color=COLORS["primary_hover"],
            **kwargs
        )


class DateEntry(ctk.CTkFrame):
    """Date entry widget with Day, Month, Year fields"""

    def __init__(self, master, **kwargs):
        super().__init__(master, fg_color="transparent", **kwargs)

        # Grid layout
        self.grid_columnconfigure((0, 1, 2), weight=1)

        # Labels
        StyledLabel(
            self, text="Day", size="small", text_color=COLORS["text_secondary"]
        ).grid(row=0, column=0, pady=(0, 2))
        StyledLabel(
            self, text="Month", size="small", text_color=COLORS["text_secondary"]
        ).grid(row=0, column=1, pady=(0, 2))
        StyledLabel(
            self, text="Year", size="small", text_color=COLORS["text_secondary"]
        ).grid(row=0, column=2, pady=(0, 2))

        # Day
        self.day_entry = StyledEntry(self, placeholder="DD", width=50)
        self.day_entry.grid(row=1, column=0, padx=(0, 5))

        # Month
        self.month_entry = StyledEntry(self, placeholder="MM", width=50)
        self.month_entry.grid(row=1, column=1, padx=5)

        # Year
        self.year_entry = StyledEntry(self, placeholder="YYYY", width=70)
        self.year_entry.grid(row=1, column=2, padx=(5, 0))

        # Set default to today
        today = datetime.datetime.now()
        self.day_entry.insert(0, str(today.day))
        self.month_entry.insert(0, str(today.month))
        self.year_entry.insert(0, str(today.year))

    def get_date(self):
        """Get date components as integers (day, month, year)"""
        try:
            day = int(self.day_entry.get())
            month = int(self.month_entry.get())
            year = int(self.year_entry.get())
            return day, month, year
        except ValueError:
            return None, None, None


class DateTimeEntry(ctk.CTkFrame):
    """Combined Date and Time entry widget"""

    def __init__(self, master, label_text="Start Time", **kwargs):
        super().__init__(master, fg_color="transparent", **kwargs)

        StyledLabel(self, text=label_text, size="body").pack(anchor="w", pady=(0, 5))

        # Container for date and time
        container = ctk.CTkFrame(self, fg_color="transparent")
        container.pack(fill="x")

        # Date
        self.date_entry = DateEntry(container)
        self.date_entry.pack(side="left", fill="x", expand=True, padx=(0, 10))

        # Time Frame (to align with DateEntry)
        time_frame = ctk.CTkFrame(container, fg_color="transparent")
        time_frame.pack(side="right", fill="y")

        StyledLabel(
            time_frame, text="Time", size="small", text_color=COLORS["text_secondary"]
        ).pack(anchor="w", pady=(0, 2))
        self.time_entry = StyledEntry(time_frame, placeholder="HH:MM", width=80)
        self.time_entry.pack(fill="x")

    def get_datetime(self):
        """Get date and time components"""
        d, m, y = self.date_entry.get_date()

        try:
            time_str = self.time_entry.get()
            if ":" in time_str:
                hour, minute = map(int, time_str.split(":"))
            else:
                hour = int(time_str)
                minute = 0

            return d, m, y, hour, minute
        except ValueError:
            return None, None, None, None, None


class TimeSlotEntry(ctk.CTkFrame):
    """Time slot entry widget for start and end times (Refactored for Date+Time)"""

    def __init__(self, master, **kwargs):
        super().__init__(master, fg_color="transparent", **kwargs)

        # Start Date/Time
        self.start_dt = DateTimeEntry(self, label_text="Start Date & Time:")
        self.start_dt.pack(fill="x", pady=(0, SPACING["md"]))

        # End Date/Time
        self.end_dt = DateTimeEntry(self, label_text="End Date & Time:")
        self.end_dt.pack(fill="x", pady=(0, SPACING["md"]))

    def get_times(self):
        """Get start and end times as seconds from start of year"""
        # Import here to avoid circular imports if any
        from utils import datetime_to_seconds

        sd, sm, sy, sh, smin = self.start_dt.get_datetime()
        ed, em, ey, eh, emin = self.end_dt.get_datetime()

        if None in (sd, sm, sy, sh, smin, ed, em, ey, eh, emin):
            return None, None

        start_seconds = datetime_to_seconds(sd, sm, sy, sh, smin)
        end_seconds = datetime_to_seconds(ed, em, ey, eh, emin)

        return start_seconds, end_seconds

    def clear(self):
        """Clear the time entries"""
        # Reset to today? Or just clear?
        pass


# Alias for backward compatibility
StyledButton = AnimatedButton
Card = ModernCard
