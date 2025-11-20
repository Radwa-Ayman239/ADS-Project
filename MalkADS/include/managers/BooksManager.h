#ifndef BOOKS_H
#define BOOKS_H

#include <string>
#include "../models/book.h"
#include "../models/user.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
using namespace std;

class BooksManager {
private:
    //Data Structure Change
    HashMap<string, Book> ID_To_BookTable;
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> BookTable;

    void loadBooksFromFile(); // read book IDs from "books.txt" and create interval tree
    void saveBooksToFile();

    void loadBookBookingsFromFile();
    void saveBookBookingsToFile() const;

public:
    BooksManager();

    ~BooksManager();

    void BorrowBook(User* user);

    // admin operations
    void addBookInteractive();

    void removeBookInteractive();

    void showUserBookings(const std::string& username) const;
};

#endif
