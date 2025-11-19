#ifndef BOOKS_H
#define BOOKS_H

#include <string>
#include "../models/book.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
using namespace std;

class BooksManager {
private:
//Data Structure Change 
    HashMap<string, Book> ID_To_BookTable;
//Data Structure Change 
    HashMap<string, RedBlackIntervalTree*> BookTable;

    void loadBooksFromFile(); // read book IDs from "books.txt" and create interval tree
    void saveBooksToFile();


public:
    BooksManager();
    ~BooksManager();

    void BorrowBook();

    // admin operations
    void addBookInteractive();
    void removeBookInteractive();
};

#endif