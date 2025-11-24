"""
Live book search dialog with real-time filtering
"""

import customtkinter as ctk
import sys
import os
from styles import COLORS, FONTS, SPACING, SIZES
from widgets import StyledButton, StyledEntry, StyledLabel, Card, ScrollableFrame

# Add parent directory to path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

try:
    import library_system
    LIBRARY_AVAILABLE = True
except ImportError:
    LIBRARY_AVAILABLE = False


class BookSearchDialog(ctk.CTkToplevel):
    """Live search dialog for books with real-time filtering"""
    
    def __init__(self, parent, lib_system=None, on_book_selected=None):
        super().__init__(parent)
        self.on_book_selected = on_book_selected
        
        # Use the shared library system instance
        self.lib_system = lib_system
        
        # Window configuration
        self.title("Search Books")
        self.geometry("700x600")
        
        # Make this window stay on top of parent
        self.transient(parent)
        self.grab_set()  # Make modal
        self.lift()  # Bring to front
        
        # Center window
        self.update_idletasks()
        x = (self.winfo_screenwidth() // 2) - (700 // 2)
        y = (self.winfo_screenheight() // 2) - (600 // 2)
        self.geometry(f'700x600+{x}+{y}')
        
        # Main container
        main_frame = Card(self, fg_color=COLORS["bg_dark"])
        main_frame.pack(expand=True, fill="both", padx=SPACING["lg"], pady=SPACING["lg"])
        
        # Header
        header = ctk.CTkFrame(main_frame, fg_color=COLORS["accent_green"], corner_radius=SIZES["button_corner"])
        header.pack(fill="x", pady=(0, SPACING["lg"]))
        
        StyledLabel(
            header,
            text="📚 Search Books",
            size="heading",
            text_color=COLORS["text_primary"]
        ).pack(pady=SPACING["md"])
        
        # Search section
        search_frame = ctk.CTkFrame(main_frame, fg_color="transparent")
        search_frame.pack(fill="x", padx=SPACING["md"], pady=(0, SPACING["md"]))
        
        StyledLabel(
            search_frame,
            text="Search by title or author (partial match supported):",
            size="body_large"
        ).pack(anchor="w", pady=(0, 5))
        
        self.search_entry = StyledEntry(search_frame, placeholder="Type to search...")
        self.search_entry.pack(fill="x")
        self.search_entry.bind("<KeyRelease>", self.on_search_changed)
        self.search_entry.focus()
        
        # Results count
        self.results_label = StyledLabel(
            search_frame,
            text="",
            size="small",
            text_color=COLORS["text_secondary"]
        )
        self.results_label.pack(anchor="w", pady=(5, 0))
        
        # Results section
        results_container = ctk.CTkFrame(main_frame, fg_color="transparent")
        results_container.pack(expand=True, fill="both", padx=SPACING["md"])
        
        StyledLabel(
            results_container,
            text="Results:",
            size="body_large"
        ).pack(anchor="w", pady=(0, 5))
        
        # Scrollable results area
        self.results_frame = ScrollableFrame(results_container)
        self.results_frame.pack(expand=True, fill="both")
        
        # Close button
        StyledButton(
            main_frame,
            text="Close",
            variant="error",
            width=150,
            command=self.destroy
        ).pack(pady=(SPACING["md"], 0))
        
        # Load and display all books initially
        self.all_books = self.load_all_books()
        self.display_books(self.all_books)
    
    def load_all_books(self):
        """Load all books from the library system"""
        if self.lib_system and LIBRARY_AVAILABLE:
            try:
                return list(self.lib_system.search_books())
            except Exception as e:
                print(f"Error loading books: {e}")
                return []
        else:
            # Mock data for development
            return [
                {"id": "B0001", "title": "Introduction to Algorithms", "author": "Cormen"},
                {"id": "B0002", "title": "Data Structures and Algorithms", "author": "Goodrich"},
                {"id": "B0003", "title": "The Algorithm Design Manual", "author": "Skiena"},
                {"id": "B0004", "title": "Database System Concepts", "author": "Silberschatz"},
                {"id": "B0005", "title": "Operating System Concepts", "author": "Silberschatz"},
            ]
    
    def on_search_changed(self, event=None):
        """Handle search text change - filter books in real-time"""
        search_text = self.search_entry.get().lower()
        
        if not search_text:
            # Show all books if search is empty
            self.display_books(self.all_books)
        else:
            # Filter books by partial match in title or author
            filtered_books = [
                book for book in self.all_books
                if search_text in book["title"].lower() or search_text in book["author"].lower()
            ]
            self.display_books(filtered_books)
    
    def display_books(self, books):
        """Display the list of books"""
        # Clear existing results
        for widget in self.results_frame.winfo_children():
            widget.destroy()
        
        # Update results count
        count = len(books)
        if count == 0:
            self.results_label.configure(text="No books found")
        elif count == 1:
            self.results_label.configure(text="1 book found")
        else:
            self.results_label.configure(text=f"{count} books found")
        
        if not books:
            # Show "no results" message
            no_results_frame = Card(self.results_frame, fg_color=COLORS["bg_medium"])
            no_results_frame.pack(fill="x", pady=SPACING["sm"], padx=SPACING["sm"])
            
            StyledLabel(
                no_results_frame,
                text="📭 No books match your search",
                size="body",
                text_color=COLORS["text_secondary"]
            ).pack(pady=SPACING["lg"])
            return
        
        # Display each book
        for book in books:
            book_card = Card(self.results_frame, fg_color=COLORS["bg_medium"])
            book_card.pack(fill="x", pady=(0, SPACING["sm"]), padx=SPACING["sm"])
            
            content_frame = ctk.CTkFrame(book_card, fg_color="transparent")
            content_frame.pack(fill="x", padx=SPACING["md"], pady=SPACING["sm"])
            
            # Book ID
            id_label = StyledLabel(
                content_frame,
                text=f"ID: {book['id']}",
                size="small",
                text_color=COLORS["accent_blue"]
            )
            id_label.pack(anchor="w")
            
            # Title
            title_label = StyledLabel(
                content_frame,
                text=f"📖 {book['title']}",
                size="body_large",
                text_color=COLORS["text_primary"]
            )
            title_label.pack(anchor="w", pady=(2, 0))
            
            # Author
            author_label = StyledLabel(
                content_frame,
                text=f"✍️ {book['author']}",
                size="body",
                text_color=COLORS["text_secondary"]
            )
            author_label.pack(anchor="w", pady=(2, 0))
            
            # Select button if callback provided
            if self.on_book_selected:
                StyledButton(
                    book_card,
                    text="Select",
                    variant="primary",
                    width=120,
                    command=lambda b=book: self.select_book(b)
                ).pack(pady=(0, SPACING["sm"]), padx=SPACING["md"])
    
    def select_book(self, book):
        """Handle book selection"""
        if self.on_book_selected:
            self.on_book_selected(book["id"], book["title"], book["author"])
        self.destroy()
