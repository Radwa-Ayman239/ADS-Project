"""
Admin dashboard with management capabilities
"""

import customtkinter as ctk
from styles import COLORS, FONTS, SPACING, SIZES, ANIMATIONS
from widgets import AnimatedButton, StyledLabel, ModernCard
from booking_forms import BookingDialog
from admin_forms import ManagementDialog
from bookings_view import BookingsView


class AdminDashboard(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color=COLORS["bg_dark"])
        self.controller = controller
        self.cards = []  # Store card references for animation

        # Header
        header = ctk.CTkFrame(self, fg_color=COLORS["secondary"], height=80)
        header.pack(fill="x", side="top")
        header.pack_propagate(False)

        # Header content
        header_content = ctk.CTkFrame(header, fg_color="transparent")
        header_content.pack(
            expand=True, fill="both", padx=SPACING["xl"], pady=SPACING["md"]
        )

        # Welcome message with admin badge
        welcome_frame = ctk.CTkFrame(header_content, fg_color="transparent")
        welcome_frame.pack(side="left")

        self.welcome_label = StyledLabel(
            welcome_frame, text="", size="heading", text_color=COLORS["text_primary"]
        )
        self.welcome_label.pack(side="left")

        admin_badge = ctk.CTkFrame(
            welcome_frame, fg_color=COLORS["warning"], corner_radius=5
        )
        admin_badge.pack(side="left", padx=SPACING["md"])

        StyledLabel(
            admin_badge,
            text="👑 ADMIN",
            size="small",
            text_color=COLORS["text_primary"],
        ).pack(padx=SPACING["sm"], pady=2)

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

        # Main content with tabs
        content = ctk.CTkFrame(self, fg_color="transparent")
        content.pack(expand=True, fill="both", padx=SPACING["xl"], pady=SPACING["xl"])

        # Tab view
        self.tabview = ctk.CTkTabview(content, fg_color=COLORS["bg_card"])
        self.tabview.pack(expand=True, fill="both")

        # Booking actions tab
        booking_tab = self.tabview.add("📅 Bookings")
        self.create_booking_section(booking_tab)

        # Management tab
        management_tab = self.tabview.add("⚙️ Management")
        self.create_management_section(management_tab)

    def create_booking_section(self, parent):
        """Create booking actions section"""
        # Grid layout
        parent.grid_columnconfigure(0, weight=1)
        parent.grid_columnconfigure(1, weight=1)

        self.create_action_card(
            parent,
            "🏛️ Book a Room",
            "Reserve a study room",
            COLORS["accent_blue"],
            lambda: self.open_booking_dialog("room"),
            row=0,
            column=0,
        )

        self.create_action_card(
            parent,
            "💻 Borrow Laptop",
            "Borrow a laptop",
            COLORS["accent_purple"],
            lambda: self.open_booking_dialog("laptop"),
            row=0,
            column=1,
        )

        self.create_action_card(
            parent,
            "📚 Borrow Book",
            "Borrow a book",
            COLORS["accent_green"],
            lambda: self.open_booking_dialog("book"),
            row=1,
            column=0,
        )

        self.create_action_card(
            parent,
            "📋 My Bookings",
            "View your reservations",
            COLORS["accent_orange"],
            self.view_my_bookings,
            row=1,
            column=1,
        )

    def create_management_section(self, parent):
        """Create resource management section"""
        # Grid layout
        parent.grid_columnconfigure(0, weight=1)
        parent.grid_columnconfigure(1, weight=1)
        parent.grid_columnconfigure(2, weight=1)

        # Books management
        self.create_action_card(
            parent,
            "📚 Add Book",
            "Add a new book to library",
            COLORS["success"],
            lambda: self.open_management_dialog("add_book"),
            row=0,
            column=0,
        )

        self.create_action_card(
            parent,
            "📚 Remove Book",
            "Remove a book",
            COLORS["error"],
            lambda: self.open_management_dialog("remove_book"),
            row=0,
            column=1,
        )

        # Laptops management
        self.create_action_card(
            parent,
            "💻 Add Laptop",
            "Add a new laptop",
            COLORS["success"],
            lambda: self.open_management_dialog("add_laptop"),
            row=1,
            column=0,
        )

        self.create_action_card(
            parent,
            "💻 Remove Laptop",
            "Remove a laptop",
            COLORS["error"],
            lambda: self.open_management_dialog("remove_laptop"),
            row=1,
            column=1,
        )

        # Rooms management
        self.create_action_card(
            parent,
            "🏛️ Add Room",
            "Add a new room",
            COLORS["success"],
            lambda: self.open_management_dialog("add_room"),
            row=2,
            column=0,
        )

        self.create_action_card(
            parent,
            "🏛️ Remove Room",
            "Remove a room",
            COLORS["error"],
            lambda: self.open_management_dialog("remove_room"),
            row=2,
            column=1,
        )

    def create_action_card(
        self, parent, title, description, color, command, row, column
    ):
        """Create an action card"""
        card = ModernCard(parent, height=150)
        card.grid(
            row=row,
            column=column,
            padx=SPACING["md"],
            pady=SPACING["md"],
            sticky="nsew",
        )

        # Store for animation
        self.cards.append(card)
        card.grid_remove()

        # Icon and title
        title_label = StyledLabel(card, text=title, size="subheading", text_color=color)
        title_label.pack(pady=(SPACING["md"], SPACING["sm"]))

        # Description
        desc_label = StyledLabel(
            card, text=description, size="body", text_color=COLORS["text_secondary"]
        )
        desc_label.pack(pady=(0, SPACING["sm"]))

        # Action button
        AnimatedButton(
            card, text="Open", variant="primary", width=150, command=command
        ).pack(pady=(SPACING["sm"], SPACING["md"]))

    def animate_cards(self):
        """Animate cards appearing one by one"""
        # Only animate cards in the currently selected tab
        # This is a simplification; ideally we'd track which tab is active
        # For now, we'll just animate all and let grid_remove/grid handle visibility
        for i, card in enumerate(self.cards):
            self.after(i * 50, lambda c=card: self.show_card(c))

    def show_card(self, card):
        """Show a single card with fade-in effect (simulated)"""
        card.grid()
        card.configure(fg_color=COLORS["bg_card"])

    def open_booking_dialog(self, booking_type):
        """Open booking dialog"""
        dialog = BookingDialog(
            self, booking_type, self.controller.current_user, self.controller.lib_system
        )
        dialog.grab_set()

    def open_management_dialog(self, action_type):
        """Open management dialog"""
        dialog = ManagementDialog(self, action_type)
        dialog.grab_set()

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
                text=f"Welcome, {self.controller.current_user}!"
            )
            # Trigger animation
            self.animate_cards()
