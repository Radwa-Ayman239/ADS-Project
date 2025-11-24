"""
Custom reusable widgets with consistent styling
"""

import customtkinter as ctk
from styles import COLORS, FONTS, SIZES, SPACING


class StyledButton(ctk.CTkButton):
    """Custom button with consistent styling and hover effects"""
    
    def __init__(self, master, text="Button", variant="primary", **kwargs):
        # Set default styling based on variant
        if variant == "primary":
            fg_color = COLORS["primary"]
            hover_color = COLORS["primary_hover"]
        elif variant == "secondary":
            fg_color = COLORS["secondary"]
            hover_color = COLORS["secondary_hover"]
        elif variant == "success":
            fg_color = COLORS["success"]
            hover_color = COLORS["success_hover"]
        elif variant == "error":
            fg_color = COLORS["error"]
            hover_color = COLORS["error_hover"]
        elif variant == "warning":
            fg_color = COLORS["warning"]
            hover_color = COLORS["warning_hover"]
        else:
            fg_color = COLORS["bg_light"]
            hover_color = COLORS["bg_medium"]
        
        super().__init__(
            master,
            text=text,
            fg_color=fg_color,
            hover_color=hover_color,
            font=FONTS["button"],
            height=SIZES["button_height"],
            corner_radius=SIZES["button_corner"],
            **kwargs
        )


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
            **kwargs
        )


class StyledLabel(ctk.CTkLabel):
    """Custom label with consistent styling"""
    
    def __init__(self, master, text="", size="body", **kwargs):
        super().__init__(
            master,
            text=text,
            font=FONTS.get(size, FONTS["body"]),
            **kwargs
        )


class Card(ctk.CTkFrame):
    """Card container with shadow effect"""
    
    def __init__(self, master, **kwargs):
        # Set default fg_color if not provided
        if 'fg_color' not in kwargs:
            kwargs['fg_color'] = COLORS["bg_card"]
        if 'corner_radius' not in kwargs:
            kwargs['corner_radius'] = SIZES["card_corner"]
        
        super().__init__(master, **kwargs)


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
            master,
            fg_color=fg_color,
            corner_radius=SIZES["button_corner"],
            **kwargs
        )
        
        self.label = StyledLabel(
            self,
            text=message,
            text_color=COLORS["text_primary"]
        )
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
        super().__init__(
            master,
            fg_color=COLORS["bg_card"],
            corner_radius=SIZES["card_corner"],
            **kwargs
        )


class TimeSlotEntry(ctk.CTkFrame):
    """Time slot entry widget for start and end times"""
    
    def __init__(self, master, **kwargs):
        super().__init__(master, fg_color="transparent", **kwargs)
        
        # Start time
        StyledLabel(self, text="Start Time (0-23):", size="body").pack(anchor="w", pady=(0, 5))
        self.start_entry = StyledEntry(self, placeholder="e.g., 9")
        self.start_entry.pack(fill="x", pady=(0, SPACING["md"]))
        
        # End time
        StyledLabel(self, text="End Time (1-24):", size="body").pack(anchor="w", pady=(0, 5))
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
