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
    void saveBooksToFile();

    void loadBookBookingsFromFile();

    void saveBookBookingsToFile() const;

public:
    BooksManager();

    ~BooksManager();

    void BorrowBook(User *user);

    // admin operations
    void addBookInteractive();

    void removeBookInteractive();

    void showUserBookings(const std::string &username) const;

    void syncUserBookings(UsersManager &usersManager);

    void parseDate(string stringdate, int &day, int &month, int &year);
    void parseTime(string stringdate, int &hour, int &minute);

    // Converts a date/time into seconds since Jan 1 2025, 00:00
    long long getUserDateAsSeconds(int &day, int &month, int &year, int &hour, int &minute);
};

#endif
