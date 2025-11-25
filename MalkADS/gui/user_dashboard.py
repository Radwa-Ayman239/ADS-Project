"""
User dashboard for regular library users
"""

import customtkinter as ctk
from styles import COLORS, FONTS, SPACING, SIZES, ANIMATIONS
from widgets import AnimatedButton, StyledLabel, ModernCard, ScrollableFrame
from booking_forms import BookingDialog
from bookings_view import BookingsView


class UserDashboard(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color=COLORS["bg_dark"])
        self.controller = controller
        self.cards = []  # Store card references for animation

        # Header
        header = ctk.CTkFrame(self, fg_color=COLORS["primary"], height=80)
        header.pack(fill="x", side="top")
        header.pack_propagate(False)

        # Header content
        header_content = ctk.CTkFrame(header, fg_color="transparent")
        header_content.pack(
            expand=True, fill="both", padx=SPACING["xl"], pady=SPACING["md"]
        )

        # Welcome message
        self.welcome_label = StyledLabel(
            header_content, text="", size="heading", text_color=COLORS["text_primary"]
        )
        self.welcome_label.pack(side="left")

        # Logout button
        AnimatedButton(
            header_content,
            text="Logout",
            variant="error",
            width=120,
            command=self.handle_logout,
        ).pack(side="right")

        # Theme toggle
        ctk.CTkButton(
            header_content,
            text="🌗",
            width=40,
            height=40,
            fg_color="transparent",
            text_color=COLORS["text_primary"],
            font=("Roboto", 20),
            hover_color=COLORS["primary_hover"],
            command=self.controller.toggle_theme,
        ).pack(side="right", padx=SPACING["sm"])

        # Main content area
        content = ScrollableFrame(self, fg_color="transparent")
        content.pack(expand=True, fill="both", padx=SPACING["xl"], pady=SPACING["xl"])

        # Grid layout for cards
        content.grid_columnconfigure(0, weight=1)
        content.grid_columnconfigure(1, weight=1)

        # Action cards
        self.create_action_card(
            content,
            "🏛️ Book a Room",
            "Reserve a study room for your session",
            COLORS["accent_blue"],
            lambda: self.open_booking_dialog("room"),
            row=0,
            column=0,
        )

        self.create_action_card(
            content,
            "💻 Borrow Laptop",
            "Borrow a laptop for your work",
            COLORS["accent_purple"],
            lambda: self.open_booking_dialog("laptop"),
            row=0,
            column=1,
        )

        self.create_action_card(
            content,
            "📚 Borrow Book",
            "Borrow a book from our collection",
            COLORS["accent_green"],
            lambda: self.open_booking_dialog("book"),
            row=1,
            column=0,
        )

        self.create_action_card(
            content,
            "📋 My Bookings",
            "View all your current reservations",
            COLORS["accent_orange"],
            self.view_my_bookings,
            row=1,
            column=1,
        )

    def create_action_card(
        self, parent, title, description, color, command, row, column
    ):
        """Create an action card"""
        card = ModernCard(parent, height=180)
        card.grid(
            row=row,
            column=column,
            padx=SPACING["md"],
            pady=SPACING["md"],
            sticky="nsew",
        )

        # Store for animation
        self.cards.append(card)
        card.grid_remove()  # Hide initially

        # Icon and title
        title_label = StyledLabel(card, text=title, size="subheading", text_color=color)
        title_label.pack(pady=(SPACING["lg"], SPACING["sm"]))

        # Description
        desc_label = StyledLabel(
            card, text=description, size="body", text_color=COLORS["text_secondary"]
        )
        desc_label.pack(pady=(0, SPACING["md"]))

        # Action button
        AnimatedButton(
            card, text="Open", variant="primary", width=200, command=command
        ).pack(pady=(SPACING["sm"], SPACING["md"]))

    def animate_cards(self):
        """Animate cards appearing one by one"""
        for i, card in enumerate(self.cards):
            self.after(i * 100, lambda c=card: self.show_card(c))

    def show_card(self, card):
        """Show a single card with fade-in effect (simulated)"""
        card.grid()
        card.configure(fg_color=COLORS["bg_card"])

    def open_booking_dialog(self, booking_type):
        """Open the booking dialog for a specific type"""
        dialog = BookingDialog(
            self, booking_type, self.controller.current_user, self.controller.lib_system
        )
        dialog.grab_set()  # Make dialog modal

    def view_my_bookings(self):
        """Open bookings view window"""
        bookings_window = BookingsView(
            self, self.controller.current_user, self.controller.lib_system
        )
        bookings_window.grab_set()

    def handle_logout(self):
        """Handle logout"""
        self.controller.logout()

    def refresh(self):
        """Refresh the dashboard with current user info"""
        if self.controller.current_user:
            self.welcome_label.configure(
                text=f"Welcome, {self.controller.current_user}! 👋"
            )
            # Trigger animation on refresh (when screen is shown)
            self.animate_cards()
