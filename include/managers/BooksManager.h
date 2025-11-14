//
// Created by hamdy on 11/14/2025.
//

#ifndef ADS_PROJECT_BOOKSMANAGER_H
#define ADS_PROJECT_BOOKSMANAGER_H
#include <iostream>

#include "hash_map.h"
#include "models/Book.h"
#include "UserManager.h"
#include "models/User.h"
using namespace std;


class BooksManager {
private:
    HashMap<string, Book *> books;
    UserManager* user_manager_;     // non-owning; LibrarySystem own it

public:
    explicit BooksManager(UserManager* um);
    ~BooksManager();
    bool addBook(const string& id, const string& title);
    Book* findBook(const string& id) const;

    bool borrowNow(User* user, const string& bookId, int todayDay);
    bool returnBook(User* user, const string& bookId, int todayDay) const;

    void listBooks();
};


#endif //ADS_PROJECT_BOOKSMANAGER_H
