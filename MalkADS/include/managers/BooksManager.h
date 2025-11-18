#ifndef BOOKS_H
#define BOOKS_H

#include <string>
#include "../models/book.h"
#include "../structures/IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class BooksManager {
private:
//Data Structure Change 
    unordered_map<string, Book> ID_To_BookTable; 
//Data Structure Change 
    unordered_map<string, RedBlackIntervalTree*> BookTable; 


public:
    BooksManager();
    ~BooksManager();

    void loadBooksFromFile(); // read book IDs from "books.txt" and create interval tree

    void BorrowBook();
};

#endif