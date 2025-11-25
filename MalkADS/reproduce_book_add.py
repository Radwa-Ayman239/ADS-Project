import sys
import os
import time

# Add current directory to path
sys.path.append(os.getcwd())

try:
    import library_system

    print("Library system imported successfully")
except ImportError as e:
    print(f"Failed to import library_system: {e}")
    sys.exit(1)


def test_add_book():
    print("Creating LibrarySystem...")
    lib = library_system.LibrarySystem()

    book_id = "TEST_B999"
    title = "Test Book Title"
    author = "Test Author"

    print(f"Adding book: {book_id}, {title}, {author}")
    lib.add_book(book_id, title, author)

    print("Forcing save...")
    lib.save()

    print("Checking file content...")
    with open("data/books.txt", "r") as f:
        content = f.read()
        if book_id in content:
            print("SUCCESS: Book ID found in file.")
            if f"{book_id},{title},{author}" in content:
                print("SUCCESS: Full book details found.")
            else:
                print("FAILURE: Book details mismatch.")
                print(f"Content snippet: {content[-100:]}")
        else:
            print("FAILURE: Book ID not found in file.")


if __name__ == "__main__":
    test_add_book()
