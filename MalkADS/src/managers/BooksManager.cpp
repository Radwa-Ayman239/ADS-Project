#include "../../include/managers/BooksManager.h"
#include "../../include/helpers/UIHelpers.h"
// #include "book.h"
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

BooksManager::BooksManager() {
  loadBooksFromFile();
  loadBookBookingsFromFile();
}

BooksManager::~BooksManager() {
  saveBooksToFile();
  saveBookBookingsToFile();
  // Data Structure Change
  BookTable.forEach([](const string &id, RedBlackIntervalTree *&tree) {
    delete tree;
    tree = nullptr;
  });
  BookTable.clear();

  // Clear Author Table
  Author_To_BooksTable.forEach(
      [](const string &author, LinkedList<string> *&list) {
        delete list;
        list = nullptr;
      });
  Author_To_BooksTable.clear();
}

void BooksManager::loadBooksFromFile() {
  ifstream file("data\\books.txt");
  if (!file) {
    cout << "Error opening books.txt\n";
    return;
  }

  string line;
  string bookID;
  string bookTitle;
  string bookAuthor;

  int commaPos1;
  int commaPos2;

  while (getline(file, line)) {
    commaPos1 = line.find(',');
    if (commaPos1 != string::npos) {
      commaPos2 = line.find(',', commaPos1 + 1);

      if (commaPos2 != string::npos) {
        bookID = line.substr(0, commaPos1);
        bookTitle = line.substr(commaPos1 + 1, commaPos2 - (commaPos1 + 1));
        bookAuthor = line.substr(commaPos2 + 1);
      }
    }

    for (char &c : bookTitle)
      c = tolower(c);

    auto newBook = Book(bookID, bookTitle, bookAuthor);
    // Data Structure Change
    ID_To_BookTable.putNew(bookTitle, newBook);

    auto *tree = new RedBlackIntervalTree();
    // Data Structure Change
    BookTable.putNew(bookTitle, tree);

    // Update Secondary Index
    string authorLower = bookAuthor;
    for (char &c : authorLower)
      c = tolower(c);

    LinkedList<string> **listPtr = Author_To_BooksTable.get(authorLower);
    if (listPtr && *listPtr) {
      (*listPtr)->push_back(bookTitle);
    } else {
      auto *newList = new LinkedList<string>();
      newList->push_back(bookTitle);
      Author_To_BooksTable.putNew(authorLower, newList);
    }
  }

  file.close();
}

void BooksManager::saveBooksToFile() {
  ofstream file("data/books.txt", ios::out | ios::trunc);
  if (!file) {
    cout << "Error opening books.txt for writing\n";
    return;
  }

  const_cast<HashMap<string, Book> &>(ID_To_BookTable)
      .forEach([&](const string &id, Book &book) {
        file << id << "," << book.getTitle() << "," << book.getAuthor() << "\n";
      });

  file.close();
}

void BooksManager::BorrowBook(User *user) {
  printSectionHeader("Borrow a Book");

  while (true) {
    int searchmethod;
    cout << COLOR_MENU << "\nYou can search for a book:\n"
         << "  1. By Title\n"
         << "  2. By Author\n"
         << "  0. Return to Main Menu\n\n"
         << COLOR_RESET;
    cout << COLOR_PROMPT << "How would you like to search? " << COLOR_RESET;
    if (!(cin >> searchmethod)) {
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }

    if (searchmethod == 0)
      return;

    string foundbookID = "";
    string foundBookTitle = "";
    string foundBookAuthor = "";
    bool bookfound = false;

    if (searchmethod == 1) {
      // Search by Title
      string titlesearch;
      cout << COLOR_PROMPT
           << "\nEnter the title of the book you are searching for: "
           << COLOR_RESET;
      cin.ignore();
      getline(cin, titlesearch);

      string titlesearchLower = titlesearch;
      for (char &c : titlesearchLower)
        c = tolower(c);

      Book *foundBook = ID_To_BookTable.get(titlesearchLower);

      if (foundBook == nullptr) {
        printError("Sorry, the library does not have this book.");
        continue;
      } else
        bookfound = true;

      foundbookID = foundBook->getID();
      foundBookTitle = foundBook->getTitle();
      foundBookAuthor = foundBook->getAuthor();

    } else if (searchmethod == 2) {
      // Search by Author
      string searchAuthor;
      cout << COLOR_PROMPT
           << "\nEnter the author you are searching for: " << COLOR_RESET;
      cin.ignore();
      getline(cin, searchAuthor);

      string searchAuthorLower = searchAuthor;
      for (char &c : searchAuthorLower)
        c = tolower(c);

      bool authorfound = false;
      cout << COLOR_MENU << "\nHere are the books we have by this author:\n\n"
           << COLOR_RESET;

      LinkedList<string> **listPtr =
          Author_To_BooksTable.get(searchAuthorLower);
      if (listPtr && *listPtr) {
        authorfound = true;
        (*listPtr)->forEach(
            [&](const string &title) { cout << "  - " << title << "\n"; });
      }

      if (!authorfound) {
        printError("Sorry, we do not have any books by this author.");
        continue;
      }

      string titlesearch;
      cout << COLOR_PROMPT
           << "\nEnter the title of the book you are searching for: "
           << COLOR_RESET;
      getline(cin, titlesearch);

      string titlesearchLower = titlesearch;
      for (char &c : titlesearchLower)
        c = tolower(c);

      ID_To_BookTable.forEach([&](const string &id, Book &book) {
        string temp = book.getTitle();
        for (char &c : temp)
          c = tolower(c);
        if (!bookfound && temp == titlesearchLower) {
          bookfound = true;
          foundbookID = id;
          foundBookTitle = book.getTitle();
          foundBookAuthor = book.getAuthor();
        }
      });

      if (!bookfound) {
        printError("Sorry, the library does not have this book.");
        continue;
      }

      // Check if author matches (optional, but good for UX as per original
      // code)
      string bookAuthorLower = foundBookAuthor;
      for (char &c : bookAuthorLower)
        c = tolower(c);

      if (bookAuthorLower != searchAuthorLower) {
        cout << COLOR_ERROR << "\nNote: the author of \"" << foundBookTitle
             << "\" is " << foundBookAuthor << "."
             << "\nThis does not match the author you initially searched for."
             << COLOR_RESET << "\n";
        cout << COLOR_PROMPT
             << "Do you still want this book? (y/n): " << COLOR_RESET;
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y')
          continue;
      }
    } else {
      printError("Invalid option.");
      continue;
    }

    // Booking Process
    if (bookfound) {
      cout << "\n"
           << COLOR_TITLE << "--- Booking \"" << foundBookTitle << "\" ---"
           << COLOR_RESET << "\n";

      int sDay, sMonth, sYear, sHour, sMinute;
      int eDay, eMonth, eYear, eHour, eMinute;

      cout << "\nEnter start date and time: \n\n";
      long long startperiod =
          getUserDateAsSeconds(sDay, sMonth, sYear, sHour, sMinute);

      cout << "\nEnter end date and time: \n\n";
      long long endperiod =
          getUserDateAsSeconds(eDay, eMonth, eYear, eHour, eMinute);

      if (!user->canBookBook(startperiod, endperiod)) {
        printError("You already have 3 books borrowed during this period.");
        continue;
      }

      RedBlackIntervalTree **treePtr = BookTable.get(foundbookID);
      if (!treePtr || !(*treePtr)) {
        printError("Internal Error: No tree for this book ID.");
        continue;
      }

      RedBlackIntervalTree *tree = *treePtr;
      if (!tree->searchOverlap(startperiod, endperiod, true)) {
        tree->insert(startperiod, endperiod, user->getUsername());
        user->addBookBooking(startperiod, endperiod);

        stringstream ss;
        ss << "Book \"" << foundBookTitle
           << "\" booked successfully from: " << setw(2) << setfill('0') << sDay
           << "/" << setw(2) << setfill('0') << sMonth << "/" << sYear << " "
           << setw(2) << setfill('0') << sHour << ":" << setw(2) << setfill('0')
           << sMinute << " to " << setw(2) << setfill('0') << eDay << "/"
           << setw(2) << setfill('0') << eMonth << "/" << eYear << " "
           << setw(2) << setfill('0') << eHour << ":" << setw(2) << setfill('0')
           << eMinute;

        printSuccess(ss.str());
        printHint("Go to the library desk at the start of the booking period "
                  "and you will be given your book.");
        return; // Exit after successful booking
      } else {
        printError("Unable to borrow this book during the desired period.");
      }
    }
  }
}

// Non-interactive version for Python API
bool BooksManager::borrowBookDirect(User *user, const string &bookId,
                                    int startTime, int endTime) {
  if (!user)
    return false;

  // Check if user's book limit
  if (!user->canBookBook(startTime, endTime)) {
    return false; // User already has 3 concurrent books
  }

  // Check if book exists
  RedBlackIntervalTree **treePtr = BookTable.get(bookId);
  if (!treePtr || !(*treePtr)) {
    return false; // Book doesn't exist
  }

  RedBlackIntervalTree *tree = *treePtr;

  // Check if book is available
  if (tree->searchOverlap(startTime, endTime, false)) {
    return false; // Book conflict
  }

  // Borrow the book
  tree->insert(startTime, endTime, user->getUsername());
  user->addBookBooking(startTime, endTime);

  return true;
}

void BooksManager::addBookInteractive() {
  printSectionHeader("Add New Book");

  string id, title, author;
  cout << COLOR_PROMPT << "Enter new book ID: " << COLOR_RESET;
  cin >> id;
  cin.ignore();
  cout << COLOR_PROMPT << "Enter title: " << COLOR_RESET;
  getline(cin, title);
  cout << COLOR_PROMPT << "Enter author: " << COLOR_RESET;
  getline(cin, author);

  if (ID_To_BookTable.contains(id)) {
    printError("A book with this ID already exists.");
    return;
  }

  Book b(id, title, author);
  ID_To_BookTable.putNew(id, b);

  auto *tree = new RedBlackIntervalTree();
  BookTable.putNew(id, tree);

  // Update Secondary Index
  string authorLower = author;
  for (char &c : authorLower)
    c = tolower(c);

  LinkedList<string> **listPtr = Author_To_BooksTable.get(authorLower);
  if (listPtr && *listPtr) {
    (*listPtr)->push_back(title);
  } else {
    auto *newList = new LinkedList<string>();
    newList->push_back(title);
    Author_To_BooksTable.putNew(authorLower, newList);
  }

  printSuccess("Book added successfully.");
}

// Non-interactive version for Python API
bool BooksManager::addBookDirect(const string &bookId, const string &title,
                                 const string &author) {
  if (ID_To_BookTable.contains(bookId)) {
    return false; // Book already exists
  }

  // Create book with provided details
  Book b(bookId, title, author);
  ID_To_BookTable.putNew(bookId, b);

  auto *tree = new RedBlackIntervalTree();
  BookTable.putNew(bookId, tree);

  // Update Secondary Index
  string authorLower = author;
  for (char &c : authorLower)
    c = tolower(c);

  LinkedList<string> **listPtr = Author_To_BooksTable.get(authorLower);
  if (listPtr && *listPtr) {
    (*listPtr)->push_back(title);
  } else {
    auto *newList = new LinkedList<string>();
    newList->push_back(title);
    Author_To_BooksTable.putNew(authorLower, newList);
  }

  return true;
}

void BooksManager::removeBookInteractive() {
  printSectionHeader("Remove Book");

  string id;
  cout << COLOR_PROMPT << "Enter book ID to remove: " << COLOR_RESET;
  cin >> id;

  if (!ID_To_BookTable.contains(id)) {
    printError("No book with this ID.");
    return;
  }

  RedBlackIntervalTree **treePtr = BookTable.get(id);
  if (treePtr && *treePtr) {
    delete *treePtr;
    *treePtr = nullptr;
  }

  // Remove data from secondary index
  Book *b = ID_To_BookTable.get(id);
  if (b) {
    string author = b->getAuthor();
    string title = b->getTitle();
    string authorLower = author;
    for (char &c : authorLower)
      c = tolower(c);

    LinkedList<string> **listPtr = Author_To_BooksTable.get(authorLower);
    if (listPtr && *listPtr) {
      (*listPtr)->remove(title);
      // Optional: if list empty, remove author from map (not strictly necessary
      // but cleaner)
      if ((*listPtr)->empty()) {
        delete *listPtr;
        Author_To_BooksTable.erase(authorLower);
      }
    }
  }

  BookTable.erase(id);
  ID_To_BookTable.erase(id);

  printSuccess("Book removed.");
}

// Non-interactive version for Python API
bool BooksManager::removeBookDirect(const string &bookId) {
  if (!ID_To_BookTable.contains(bookId)) {
    return false; // Book doesn't exist
  }

  RedBlackIntervalTree **treePtr = BookTable.get(bookId);
  if (treePtr && *treePtr) {
    delete *treePtr;
    *treePtr = nullptr;
  }

  // Remove data from secondary index
  Book *b = ID_To_BookTable.get(bookId);
  if (b) {
    string author = b->getAuthor();
    string title = b->getTitle();
    string authorLower = author;
    for (char &c : authorLower)
      c = tolower(c);

    LinkedList<string> **listPtr = Author_To_BooksTable.get(authorLower);
    if (listPtr && *listPtr) {
      (*listPtr)->remove(title);
      if ((*listPtr)->empty()) {
        delete *listPtr;
        Author_To_BooksTable.erase(authorLower);
      }
    }
  }

  BookTable.erase(bookId);
  ID_To_BookTable.erase(bookId);

  return true;
}

Book *BooksManager::getBook(const string &bookId) {
  return ID_To_BookTable.get(bookId);
}

void BooksManager::loadBookBookingsFromFile() {
  loadBookingsFromFile("data/book_bookings.txt", BookTable);
}

void BooksManager::saveBookBookingsToFile() const {
  saveBookingsToFile("data/book_bookings.txt", BookTable);
}

void BooksManager::showUserBookings(const std::string &username) const {
  cout << COLOR_MENU << "\nYour book bookings:\n\n" << COLOR_RESET;

  bool any = false;
  const_cast<HashMap<string, RedBlackIntervalTree *> &>(BookTable).forEach(
      [&](const string &bookId, RedBlackIntervalTree *&tree) {
        if (!tree)
          return;

        tree->forEachInterval(
            [&](const int low, const int high, const std::string &user) {
              if (user == username) {
                Book *b = ID_To_BookTable.get(bookId);
                const string title = b ? b->getTitle() : "(unknown)";
                const string author = b ? b->getAuthor() : "(unknown)";

                cout << "  - Book ID: " << bookId << " | Title: " << title
                     << " | Author: " << author << " | Period: ["
                     << formatTimestamp(low) << " to " << formatTimestamp(high)
                     << "]\n";
                any = true;
              }
            });
      });

  if (!any)
    printHint("You have no book bookings.");
}

void BooksManager::syncUserBookings(UsersManager &usersManager) {
  BookTable.forEach([&](const string &bookId, RedBlackIntervalTree *&tree) {
    if (!tree)
      return;
    tree->forEachInterval(
        [&](const int low, const int high, const string &username) {
          User *u = usersManager.getUser(username);
          if (u)
            u->addBookBooking(low, high);
        });
  });
}
