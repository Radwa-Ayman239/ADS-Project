import customtkinter as ctk
from styles import COLORS, FONTS, SPACING, SIZES
from widgets import StyledLabel

class TimeSlotGrid(ctk.CTkFrame):
    """Widget to display and select time slots"""
    def __init__(self, parent, on_select=None):
        super().__init__(parent, fg_color="transparent")
        self.on_select = on_select
        self.selected_start = None
        self.selected_end = None
        self.booked_slots = []
        self.buttons = {}
        
        # Grid configuration
        self.grid_columnconfigure((0,1,2,3,4,5), weight=1)
        
        # Create 24 hour slots
        for hour in range(24):
            btn = ctk.CTkButton(
                self,
                text=f"{hour:02d}:00",
                width=60,
                height=30,
                fg_color=COLORS["success"],  # Default available
                hover_color=COLORS["accent_green"],
                command=lambda h=hour: self.handle_click(h)
            )
            row = hour // 6
            col = hour % 6
            btn.grid(row=row, column=col, padx=2, pady=2)
            self.buttons[hour] = btn
            
        # Legend
        legend = ctk.CTkFrame(self, fg_color="transparent")
        legend.grid(row=4, column=0, columnspan=6, pady=(10, 0))
        
        self.create_legend_item(legend, "Available", COLORS["success"]).pack(side="left", padx=5)
        self.create_legend_item(legend, "Booked", COLORS["error"]).pack(side="left", padx=5)
        self.create_legend_item(legend, "Selected", COLORS["accent_blue"]).pack(side="left", padx=5)

    def create_legend_item(self, parent, text, color):
        frame = ctk.CTkFrame(parent, fg_color="transparent")
        box = ctk.CTkFrame(frame, width=15, height=15, fg_color=color)
        box.pack(side="left", padx=(0, 5))
        label = StyledLabel(frame, text=text, size="small")
        label.pack(side="left")
        return frame

    def set_booked_slots(self, bookings):
        """Set booked slots (list of dicts with start/end)"""
        self.booked_slots = []
        # Reset all to available first
        for hour in range(24):
            self.buttons[hour].configure(fg_color=COLORS["success"], state="normal")
            
        # Mark booked slots
        for booking in bookings:
            start = booking['start']
            end = booking['end']
            for hour in range(start, end):
                if hour in self.buttons:
                    self.buttons[hour].configure(fg_color=COLORS["error"], state="disabled")
                    self.booked_slots.append(hour)
        
        # Reset selection
        self.selected_start = None
        self.selected_end = None

    def handle_click(self, hour):
        """Handle slot click"""
        if hour in self.booked_slots:
            return

        # If nothing selected or both selected, start new selection
        if self.selected_start is None or (self.selected_start is not None and self.selected_end is not None):
            self.selected_start = hour
            self.selected_end = None
        
        # If start selected, check if valid end
        elif self.selected_start is not None:
            if hour < self.selected_start:
                self.selected_start = hour
                self.selected_end = None
            else:
                # Check for overlap with bookings
                valid = True
                for h in range(self.selected_start, hour + 1):
                    if h in self.booked_slots:
                        valid = False
                        break
                
                if valid:
                    self.selected_end = hour + 1 # End is exclusive
                else:
                    # Reset if trying to book over existing booking
                    self.selected_start = hour
                    self.selected_end = None
        
        self.update_colors()
        
        if self.on_select and self.selected_start is not None:
            end = self.selected_end if self.selected_end else self.selected_start + 1
            self.on_select(self.selected_start, end)

    def update_colors(self):
        """Update button colors based on state"""
        for hour in range(24):
            if hour in self.booked_slots:
                continue
                
            if self.selected_start is not None:
                is_selected = False
                if self.selected_end is None:
                    is_selected = (hour == self.selected_start)
                else:
                    is_selected = (self.selected_start <= hour < self.selected_end)
                
                if is_selected:
                    self.buttons[hour].configure(fg_color=COLORS["accent_blue"])
                else:
                    self.buttons[hour].configure(fg_color=COLORS["success"])
            else:
                self.buttons[hour].configure(fg_color=COLORS["success"])
