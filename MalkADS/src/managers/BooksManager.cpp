#include "../../include/managers/BooksManager.h"
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
    int searchmethod;
    cout << "\nYou can search for a book either \n\n1. By Title \nor \n2. By Author";
    cout << "\n\nHow would you like to search?: ";
    cin >> searchmethod;

    //Data Structure Change
    string titlesearch = "";
    string currentTitle;
    string foundbookID;
    bool bookfound;


    if (searchmethod == 1) {
        currentTitle = "";
        foundbookID = "";
        bookfound = false;

        cout << "\nEnter the title of the book you are searching for: ";
        cin.ignore();
        getline(cin, titlesearch);

        for (char &c: titlesearch) c = static_cast<char>(tolower(c));

        //Data Structure Change
        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getTitle();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (!bookfound && temp == titlesearch) {
                bookfound = true;
                foundbookID = id;
            }
        });

        if (!bookfound) {
            cout << "\nSorry the library does not have this book";
            return;
        }

        int startperiod;
        int endperiod;
        cout << "\n\nWhen would you like to borrow this book?";
        cout << "\n\nStart of borrowing period: ";
        cin >> startperiod;
        cout << "End of borrowing period: ";
        cin >> endperiod;

        if (!user->canBookBook(startperiod, endperiod)) {
            cout << "\nYou already have 3 books borrowed during this period.\n";
            return;
        }


        //Data Structure Change
        RedBlackIntervalTree **treePtr = BookTable.get(foundbookID);
        if (!treePtr || !(*treePtr)) {
            cout << "\nInternal Error: No tree for this book ID.\n";
            return;
        }

        RedBlackIntervalTree *tree = *treePtr;
        if (!tree->searchOverlap(startperiod, endperiod, true)) {
            tree->insert(startperiod, endperiod, user->getUsername());
            user->addBookBooking(startperiod, endperiod);
            cout << "\n\nBooking successful!";
            cout <<
                    "\nGo to the library desk at the start of booking period, give your details, and you will be given your desired book";
        } else {
            cout << "\nUnable to borrow this book during the desired period.";
        }
    } else {
        string searchAuthor = "";
        string currentAuthor = "";
        bool authorfound = false;

        currentTitle = "";
        foundbookID = "";
        bookfound = false;


        cout << "\nEnter the author you are searching for: ";
        cin.ignore();
        getline(cin, searchAuthor);

        for (char &c: searchAuthor) c = static_cast<char>(tolower(c));


        //Data Structure Change
        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getAuthor();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (!authorfound && temp == searchAuthor)
                authorfound = true;
        });

        if (!authorfound) {
            cout << "\nSorry we do not have any books by this author";
            return;
        }
        cout << "\nHere are the books we have written by this author: " << endl;

        //Data Structure Change
        ID_To_BookTable.forEach([&](const string &id, Book &book) {
            string temp = book.getAuthor();
            for (char &c: temp) c = static_cast<char>(tolower(c));
            if (temp == searchAuthor)
                cout << "- " << book.getTitle() << endl;
        });

        cout << "\nEnter the title of the book you are searching for: ";
        getline(cin, titlesearch);
        string author_of_inputbook = "";

        for (char &c: titlesearch) c = static_cast<char>(tolower(c));

        //Data Structure Change
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
            cout << "\nSorry the library does not have this book.";
            return;
        }

        for (char &c: author_of_inputbook) c = static_cast<char>(tolower(c));
        bool authormatch = (author_of_inputbook == searchAuthor);

        char stillwant = 'y';
        if (!authormatch) {
            cout << "\nThe author of " << titlesearch << " is " << author_of_inputbook;
            cout << "\nThis is not the author you were originally looking for ";
            cout << "\nDo you still want the book? (y or n): ";
            cin >> stillwant;
        }

        if (authormatch || stillwant == 'y' || stillwant == 'Y') {
            int startperiod, endperiod;
            cout << "\n\nWhen would you like to borrow this book?";
            cout << "\n\nStart of borrowing period: ";
            cin >> startperiod;
            cout << "End of borrowing period: ";
            cin >> endperiod;

            RedBlackIntervalTree **treePtr = BookTable.get(foundbookID);
            if (!treePtr || !(*treePtr)) {
                cout << "\nInternal error: no tree for this book ID.\n";
                return;
            }
            RedBlackIntervalTree *tree = *treePtr;

            if (!tree->searchOverlap(startperiod, endperiod, true)) {
                tree->insert(startperiod, endperiod, user->getUsername());
                cout << "\n\nBooking successful!";
                cout <<
                        "\nGo to the library help desk at the start of booking period, give your details, and you will be given your desired book";
            } else {
                cout << "\nUnable to borrow this book during the desired period";
            }
        }


        // bool authormatch = false;
        //
        // for (int i = 0; i < author_of_inputbook.length(); i++) {
        //     author_of_inputbook[i] = tolower(author_of_inputbook[i]);
        // }
        //
        // if (author_of_inputbook == searchAuthor) {
        //     authormatch = true;
        // }
        //
        // if (bookfound == false) {
        //     cout << "\nSorry the library does not have this book";
        // } else {
        //     char stillwant;
        //     if (authormatch == false) {
        //         cout << "\nThe author of " << titlesearch << " is " << author_of_inputbook;
        //         cout << "\nThis is not the author you were originally looking for ";
        //         cout << "\nDo you still want the book? (y or n): ";
        //         cin >> stillwant;
        //     }
        //
        //     if (authormatch == true || stillwant == 'y' || stillwant == 'Y') {
        //         int startperiod;
        //         int endperiod;
        //         cout << "\n\nWhen would you like to borrow this book?";
        //         cout << "\n\nStart of borrowing period: ";
        //         cin >> startperiod;
        //         cout << "End of borrowing period: ";
        //         cin >> endperiod;
        //
        //         //Data Structure Change
        //         if (BookTable[foundbookID]->searchOverlap(startperiod, endperiod, true) == false) {
        //             BookTable[foundbookID]->insert(startperiod, endperiod);
        //             cout << "\n\nBooking successful!";
        //             cout <<
        //                     "\nGo to the library help desk at the start of booking period, give your details, and you will be given your desired book";
        //         } else {
        //             cout << "\nUnable to borrow this book during the desired period";
        //         }
        //     }
        // }
    }
}


//Borrow a book
//-Asks if user wants to search by author, or search by name - call respective function
//-Ask for interval


//Search by bookname

//Search by author

void BooksManager::addBookInteractive() {
    string id, title, author;
    cout << "\nEnter new book ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter title: ";
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);

    if (ID_To_BookTable.contains(id)) {
        cout << "A book with this ID already exists.\n";
        return;
    }

    Book b(id, title, author);
    ID_To_BookTable.putNew(id, b);

    auto *tree = new RedBlackIntervalTree();
    BookTable.putNew(id, tree);

    cout << "Book added successfully.\n";
}

void BooksManager::removeBookInteractive() {
    string id;
    cout << "\nEnter book ID to remove: ";
    cin >> id;

    if (!ID_To_BookTable.contains(id)) {
        cout << "No book with this ID.\n";
        return;
    }

    RedBlackIntervalTree **treePtr = BookTable.get(id);
    if (treePtr && *treePtr) {
        delete *treePtr;
        *treePtr = nullptr;
    }

    BookTable.erase(id);
    ID_To_BookTable.erase(id);

    cout << "Book removed.\n";
}

void BooksManager::loadBookBookingsFromFile() {
    ifstream file("data/book_bookings.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t c1 = line.find(',');
        if (c1 == string::npos) continue;
        size_t c2 = line.find(',', c1 + 1);
        if (c2 == string::npos) continue;
        size_t c3 = line.find(',', c2 + 1);
        if (c3 == string::npos) continue;

        string bookId = line.substr(0, c1);
        string startStr = line.substr(c1 + 1, c2 - (c1 + 1));
        string endStr = line.substr(c2 + 1, c3 - (c2 + 1));
        string user = line.substr(c3 + 1);

        int start = stoi(startStr);
        int end = stoi(endStr);

        RedBlackIntervalTree **treePtr = BookTable.get(bookId);
        if (!treePtr || !(*treePtr)) continue;

        RedBlackIntervalTree *tree = *treePtr;
        tree->insert(start, end, user);
    }

    file.close();
}

void BooksManager::saveBookBookingsToFile() const {
    ofstream file("data/book_bookings.txt", ios::out | ios::trunc);
    if (!file) {
        cout << "Error opening book_bookings.txt for writing\n";
        return;
    }

    const_cast<HashMap<string, RedBlackIntervalTree *> &>(BookTable).forEach(
        [&](const string &id, RedBlackIntervalTree * &tree) {
            if (!tree) return;
            tree->forEachInterval([&](const int low, const int high, const std::string &username) {
                file << id << "," << low << "," << high << "," << username << "\n";
            });
        });

    file.close();
}

void BooksManager::showUserBookings(const std::string &username) const {
    bool any = false;

    const_cast<HashMap<string, RedBlackIntervalTree *> &>(BookTable)
            .forEach([&](const string &bookId, RedBlackIntervalTree * &tree) {
                if (!tree) return;

                tree->forEachInterval([&](const int low, const int high, const std::string &user) {
                    if (user == username) {
                        Book *b = ID_To_BookTable.get(bookId);
                        const string title = b ? b->getTitle() : "(unknown)";
                        const string author = b ? b->getAuthor() : "(unknown)";

                        cout << "- Book ID:" << bookId
                                << " | Title: " << title
                                << " | Author: " << author
                                << " | Period: [" << low << ", " << high << "]\n";
                        any = true;
                    }
                });
            });

    if (!any) {
        cout << "You have no book bookings.\n";
    }
}
