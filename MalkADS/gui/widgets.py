"""
Custom reusable widgets with consistent styling and animations
"""

import customtkinter as ctk
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


class TimeSlotEntry(ctk.CTkFrame):
    """Time slot entry widget for start and end times"""

    def __init__(self, master, **kwargs):
        super().__init__(master, fg_color="transparent", **kwargs)

        # Start time
        StyledLabel(self, text="Start Time (0-23):", size="body").pack(
            anchor="w", pady=(0, 5)
        )
        self.start_entry = StyledEntry(self, placeholder="e.g., 9")
        self.start_entry.pack(fill="x", pady=(0, SPACING["md"]))

        # End time
        StyledLabel(self, text="End Time (1-24):", size="body").pack(
            anchor="w", pady=(0, 5)
        )
        self.end_entry = StyledEntry(self, placeholder="e.g., 17")
        self.end_entry.pack(fill="x", pady=(0, SPACING["md"]))

    def get_times(self):
        """Get start and end times as integers"""
        try:
            start = int(self.start_entry.get())
            end = int(self.end_entry.get())
            return start, end
        except ValueError:
            return None, None

    def clear(self):
        """Clear the time entries"""
        self.start_entry.delete(0, "end")
        self.end_entry.delete(0, "end")


# Alias for backward compatibility
StyledButton = AnimatedButton
Card = ModernCard
