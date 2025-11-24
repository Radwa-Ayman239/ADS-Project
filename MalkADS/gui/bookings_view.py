"""
View for displaying user bookings
"""

import customtkinter as ctk
import sys
import os
from styles import COLORS, FONTS, SPACING, SIZES
from widgets import StyledButton, StyledLabel, Card, ScrollableFrame

# Add parent directory to path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

try:
    import library_system
    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False


class BookingsView(ctk.CTkToplevel):
    """Window to view user bookings"""
    
    def __init__(self, parent, username, lib_system=None):
        super().__init__(parent)
        self.username = username
        self.lib_system = lib_system
        
        # Window configuration
        self.title("My Bookings")
        self.geometry("800x600")
        
        # Center window
        self.update_idletasks()
        x = (self.winfo_screenwidth() // 2) - (800 // 2)
        y = (self.winfo_screenheight() // 2) - (600 // 2)
        self.geometry(f'800x600+{x}+{y}')
        
        # Main container
        main_frame = ctk.CTkFrame(self, fg_color=COLORS["bg_dark"])
        main_frame.pack(expand=True, fill="both", padx=SPACING["lg"], pady=SPACING["lg"])
        
        # Header
        header = ctk.CTkFrame(main_frame, fg_color=COLORS["primary"], corner_radius=SIZES["button_corner"])
        header.pack(fill="x", pady=(0, SPACING["lg"]))
        
        StyledLabel(
            header,
            text=f"📋 Bookings for {username}",
            size="heading",
            text_color=COLORS["text_primary"]
        ).pack(pady=SPACING["md"])
        
        # Scrollable content
        content_frame = ScrollableFrame(main_frame)
        content_frame.pack(expand=True, fill="both", pady=(0, SPACING["md"]))
        
        # Load and display bookings
        self.display_bookings(content_frame)
        
        # Close button
        StyledButton(
            main_frame,
            text="Close",
            variant="primary",
            width=150,
            command=self.destroy
        ).pack(pady=(SPACING["sm"], 0))
    
    def display_bookings(self, parent):
        """Display all user bookings"""
        # Get bookings
        bookings = self.get_user_bookings()
        
        if not bookings:
            # No bookings message
            no_bookings_frame = Card(parent)
            no_bookings_frame.pack(fill="x", pady=SPACING["md"], padx=SPACING["md"])
            
            StyledLabel(
                no_bookings_frame,
                text="📭 No bookings found",
                size="subheading",
                text_color=COLORS["text_secondary"]
            ).pack(pady=SPACING["xl"])
            
            StyledLabel(
                no_bookings_frame,
                text="You haven't made any bookings yet.",
                size="body",
                text_color=COLORS["text_dim"]
            ).pack(pady=(0, SPACING["xl"]))
            return
        
        # Group bookings by type
        rooms = [b for b in bookings if b["type"] == "room"]
        laptops = [b for b in bookings if b["type"] == "laptop"]
        books = [b for b in bookings if b["type"] == "book"]
        
        # Display each category
        if rooms:
            self.display_category(parent, "🏛️ Room Bookings", rooms, COLORS["accent_blue"])
        
        if laptops:
            self.display_category(parent, "💻 Laptop Bookings", laptops, COLORS["accent_purple"])
        
        if books:
            self.display_category(parent, "📚 Book Bookings", books, COLORS["accent_green"])
    
    def display_category(self, parent, title, bookings, color):
        """Display a category of bookings"""
        # Category header
        header_frame = ctk.CTkFrame(parent, fg_color="transparent")
        header_frame.pack(fill="x", pady=(SPACING["md"], SPACING["sm"]), padx=SPACING["md"])
        
        StyledLabel(
            header_frame,
            text=title,
            size="subheading",
            text_color=color
        ).pack(anchor="w")
        
        # Bookings list
        for booking in bookings:
            booking_card = Card(parent)
            booking_card.pack(fill="x", pady=(0, SPACING["sm"]), padx=SPACING["md"])
            
            booking_content = ctk.CTkFrame(booking_card, fg_color="transparent")
            booking_content.pack(fill="x", padx=SPACING["md"], pady=SPACING["sm"])
            
            # Resource Info
            if booking['type'] == 'book' and 'title' in booking:
                resource_text = f"📖 {booking['title']} by {booking['author']}\nID: {booking['resource_id']}"
            else:
                resource_text = f"Resource: {booking['resource_id']}"
                
            resource_label = StyledLabel(
                booking_content,
                text=resource_text,
                size="body_large",
                text_color=COLORS["text_primary"]
            )
            resource_label.pack(anchor="w")
            
            # Time range
            time_label = StyledLabel(
                booking_content,
                text=f"Time: {booking['start_time']}:00 - {booking['end_time']}:00",
                size="body",
                text_color=COLORS["text_secondary"]
            )
            time_label.pack(anchor="w", pady=(2, 0))
    
    def get_user_bookings(self):
        """Get all bookings for the current user"""
        if self.lib_system and LIBRARY_AVAILABLE:
            try:
                bookings = self.lib_system.get_user_bookings(self.username)
                return list(bookings)
            except Exception as e:
                print(f"Error getting bookings: {e}")
                return []
        else:
            # Mock data for development
            return [
                {"type": "room", "resource_id": "R101", "start_time": 9, "end_time": 11},
                {"type": "laptop", "resource_id": "L001", "start_time": 14, "end_time": 17},
                {"type": "book", "resource_id": "B0001", "start_time": 10, "end_time": 18},
            ]
