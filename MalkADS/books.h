#ifndef BOOKS_H
#define BOOKS_H

#include <string>
#include "book.h"
#include "IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class Books {
private:
//Data Structure Change 
    unordered_map<string, Book> ID_To_BookTable; 
//Data Structure Change 
    unordered_map<string, RedBlackIntervalTree*> BookTable; 


public:
    Books();
    ~Books();

    void loadBooksFromFile(); // read book IDs from "books.txt" and create interval tree

    void BorrowBook();
};

#endif