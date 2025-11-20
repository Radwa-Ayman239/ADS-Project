#include "../../include/managers/BooksManager.h"
#include "../../include/helpers/UIHelpers.h"
//#include "book.h"
#include <fstream>
#include <iostream>
using namespace std;

BooksManager::BooksManager() {
    loadBooksFromFile();
    loadBookBookingsFromFile();
}


BooksManager::~BooksManager() {
    saveBooksToFile();
    saveBookBookingsToFile();
    //Data Structure Change
    BookTable.forEach([](const string &id, RedBlackIntervalTree * &tree) {
        delete tree;
        tree = nullptr;
    });
    BookTable.clear();
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

        auto newBook = Book(bookID, bookTitle, bookAuthor);
        //Data Structure Change
        ID_To_BookTable.putNew(bookID, newBook);

        auto *tree = new RedBlackIntervalTree();
        //Data Structure Change
        BookTable.putNew(bookID, tree);
    }

    file.close();
}

void BooksManager::saveBooksToFile() {
    ofstream file("data/books.txt", ios::out | ios::trunc);
    if (!file) {
        cout << "Error opening books.txt for writing\n";
        return;
    }

    const_cast<HashMap<string, Book> &>(ID_To_BookTable).forEach(
        [&](const string &id, Book &book) {
            file << id << "," << book.getTitle() << "," << book.getAuthor() << "\n";
        });

    file.close();
}


void BooksManager::BorrowBook(User *user) {
    printSectionHeader("Borrow a Book");

    int searchmethod;
    cout << COLOR_MENU
            << "\nYou can search for a book:\n"
            << "  1. By Title\n"
            << "  2. By Author\n\n"
            << COLOR_RESET;
    cout << COLOR_PROMPT << "How would you like to search? " << COLOR_RESET;
    cin >> searchmethod;

    string titlesearch = "";
    string currentTitle;
    string foundbookID;
    bool bookfound = false;

    if (searchmethod == 1) {
        currentTitle = "";
        foundbookID = "";
        bookfound = false;

        cout << COLOR_PROMPT << "\nEnter the title of the book you are searching for: "
                << COLOR_RESET;
        cin.ignore();
        getline(cin, titlesearch);

        for (char &c: titlesearch) c = static_cast<char>(tolower(c));

        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getTitle();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (!bookfound && temp == titlesearch) {
                bookfound = true;
                foundbookID = id;
            }
        });

        if (!bookfound) {
            printError("Sorry, the library does not have this book.");
            return;
        }

        int startperiod;
        int endperiod;
        cout << COLOR_PROMPT << "\nStart of borrowing period: " << COLOR_RESET;
        cin >> startperiod;
        cout << COLOR_PROMPT << "End of borrowing period: " << COLOR_RESET;
        cin >> endperiod;

        if (!user->canBookBook(startperiod, endperiod)) {
            printError("You already have 3 books borrowed during this period.");
            return;
        }

        RedBlackIntervalTree **treePtr = BookTable.get(foundbookID);
        if (!treePtr || !(*treePtr)) {
            printError("Internal Error: No tree for this book ID.");
            return;
        }

        RedBlackIntervalTree *tree = *treePtr;
        if (!tree->searchOverlap(startperiod, endperiod, true)) {
            tree->insert(startperiod, endperiod, user->getUsername());
            user->addBookBooking(startperiod, endperiod);
            printSuccess("Booking successful!");
            printHint("Go to the library desk at the start of the booking period "
                "and you will be given your book.");
        } else {
            printError("Unable to borrow this book during the desired period.");
        }
    } else {
        string searchAuthor = "";
        string currentAuthor = "";
        bool authorfound = false;

        currentTitle = "";
        foundbookID = "";
        bookfound = false;

        cout << COLOR_PROMPT << "\nEnter the author you are searching for: " << COLOR_RESET;
        cin.ignore();
        getline(cin, searchAuthor);

        for (char &c: searchAuthor) c = static_cast<char>(tolower(c));

        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getAuthor();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (!authorfound && temp == searchAuthor)
                authorfound = true;
        });

        if (!authorfound) {
            printError("Sorry, we do not have any books by this author.");
            return;
        }

        cout << COLOR_MENU << "\nHere are the books we have by this author:\n\n"
                << COLOR_RESET;
        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getAuthor();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (temp == searchAuthor)
                cout << "  - " << book.getTitle() << "\n";
        });

        cout << COLOR_PROMPT << "\nEnter the title of the book you are searching for: "
                << COLOR_RESET;
        getline(cin, titlesearch);
        string author_of_inputbook = "";

        for (char &c: titlesearch) c = static_cast<char>(tolower(c));

        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string TempTitle = book.getTitle();
            for (char &c: TempTitle) c = static_cast<char>(tolower(c));
            if (!bookfound && TempTitle == titlesearch) {
                bookfound = true;
                foundbookID = id;
                author_of_inputbook = book.getAuthor();
            }
        });

        if (!bookfound) {
            printError("Sorry, the library does not have this book.");
            return;
        }

        for (char &c: author_of_inputbook) c = static_cast<char>(tolower(c));
        bool authormatch = (author_of_inputbook == searchAuthor);

        char stillwant = 'y';
        if (!authormatch) {
            cout << COLOR_ERROR
                    << "\nNote: the author of \"" << titlesearch << "\" is "
                    << author_of_inputbook << "."
                    << "\nThis does not match the author you initially searched for."
                    << COLOR_RESET << "\n";
            cout << COLOR_PROMPT << "Do you still want this book? (y or n): "
                    << COLOR_RESET;
            cin >> stillwant;
        }

        if (authormatch || stillwant == 'y' || stillwant == 'Y') {
            int startperiod, endperiod;
            cout << COLOR_PROMPT << "\nStart of borrowing period: " << COLOR_RESET;
            cin >> startperiod;
            cout << COLOR_PROMPT << "End of borrowing period: " << COLOR_RESET;
            cin >> endperiod;

            if (!user->canBookBook(startperiod, endperiod)) {
                printError("You already have 3 books borrowed during this period.");
                return;
            }

            RedBlackIntervalTree **treePtr = BookTable.get(foundbookID);
            if (!treePtr || !(*treePtr)) {
                printError("Internal error: no tree for this book ID.");
                return;
            }
            RedBlackIntervalTree *tree = *treePtr;

            if (!tree->searchOverlap(startperiod, endperiod, true)) {
                tree->insert(startperiod, endperiod, user->getUsername());
                user->addBookBooking(startperiod, endperiod);
                printSuccess("Booking successful!");
                printHint("Go to the library help desk at the start of the booking period "
                    "and you will be given your book.");
            } else {
                printError("Unable to borrow this book during the desired period.");
            }
        }
    }
}


//Borrow a book
//-Asks if user wants to search by author, or search by name - call respective function
//-Ask for interval


//Search by bookname

//Search by author

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

    printSuccess("Book added successfully.");
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

    BookTable.erase(id);
    ID_To_BookTable.erase(id);

    printSuccess("Book removed.");
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
    const_cast<HashMap<string, RedBlackIntervalTree *> &>(BookTable)
            .forEach([&](const string &bookId, RedBlackIntervalTree * &tree) {
                if (!tree) return;

                tree->forEachInterval([&](const int low, const int high, const std::string &user) {
                    if (user == username) {
                        Book *b = ID_To_BookTable.get(bookId);
                        const string title = b ? b->getTitle() : "(unknown)";
                        const string author = b ? b->getAuthor() : "(unknown)";

                        cout << "  - Book ID: " << bookId
                                << " | Title: " << title
                                << " | Author: " << author
                                << " | Period: [" << low << ", " << high << "]\n";
                        any = true;
                    }
                });
            });

    if (!any) printHint("You have no book bookings.");
}
