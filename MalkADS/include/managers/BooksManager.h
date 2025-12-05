#ifndef BOOKS_H
#define BOOKS_H

#include <string>

#include "UsersManager.h"
#include "../models/book.h"
#include "../models/user.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
#include "../helpers/ResourceIO.h"
using namespace std;

class BooksManager {
private:
    //Data Structure Change
    HashMap<string, Book> ID_To_BookTable;
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> BookTable;

    void loadBooksFromFile(); // read book IDs from "books.txt" and create interval tree
    
    void loadBookBookingsFromFile();

    

public:
    BooksManager();

    ~BooksManager();

    void BorrowBook(User *user);
    
    // Non-interactive version for Python API
    bool borrowBookDirect(User *user, const string& bookId, int startTime, int endTime);

    // admin operations
    void addBookInteractive();
    
    // Non-interactive version for Python API
    bool addBookDirect(const string& bookId, const string& title, const string& author);

    void removeBookInteractive();
    
    // Non-interactive version for Python API
    bool removeBookDirect(const string& bookId);
    
    // Get book details
    Book* getBook(const string& bookId);
    
    // Save methods - made public for Python access
    void saveBooksToFile();
    void saveBookBookingsToFile() const;

    void showUserBookings(const std::string &username) const;

    void syncUserBookings(UsersManager &usersManager);
    
    // Iterator for Python bindings
    template<typename Func>
    void forEachBooking(Func func) {
        BookTable.forEach([&](const string& bookId, RedBlackIntervalTree*& tree) {
            if (!tree) return;
            tree->forEachInterval([&](int low, int high, const string& username) {
                func(bookId, low, high, username);
            });
        });
    }
    
    // Get list of all resource IDs
    template<typename Func>
    void forEachBook(Func func) {
        ID_To_BookTable.forEach([&](const string& bookId, Book& book) {
            func(bookId);
        });
    }
    
    // Get all book details (ID, title, author) for search
    template<typename Func>
    void forEachBookWithDetails(Func func) {
        ID_To_BookTable.forEach([&](const string& bookId, Book& book) {
            func(bookId, book.getTitle(), book.getAuthor());
        });
    }
};

#endif
