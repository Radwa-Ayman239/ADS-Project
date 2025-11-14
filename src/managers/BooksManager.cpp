//
// Created by hamdy on 11/14/2025.
//

#include "managers/BooksManager.h"
#include <iostream>
using namespace std;

BooksManager::BooksManager(UserManager *um) : user_manager_(um) {
}

BooksManager::~BooksManager() {
    books.forEach([](const string &id, const Book *b) {
        delete b;
    });
    books.clear();
}

bool BooksManager::addBook(const string &id, const string &title) {
    if (books.contains(id)) {
        cout << "Book with this id already exists.\n";
        return false;
    }

    const auto b = new Book(id, title);
    if (!books.putNew(id, b)) {
        delete b;
        return false;
    }

    return true;
}

Book *BooksManager::findBook(const string &id) const {
    return books.get(id);
}

bool BooksManager::borrowNow(User *user, const string &bookId, const int todayDay) {
    if (!user) {
        cout << "No user logged in.\n";
        return false;
    }

    Book *b = books.get(bookId);
    if (!b) {
        cout << "Book not found.\n";
        return false;
    }
    if (b->isOnLoan()) {
        cout << "Book is already in loan.\n";
        return false;
    }

    const Day start{todayDay};
    constexpr int loanLength = 30;
    const Day due{todayDay + loanLength};

    b->startLoan(user->getUsername(), start, due);

    if (user_manager_) {
        UserManager::notify(user, "Borrowed '" + b->getTitle() +
                                  "', due in " + to_string(loanLength) + " days.");
    }

    return true;
}

bool BooksManager::returnBook(User *user, const string &bookId, const int todayDay) const {
    (void) todayDay;

    if (!user) {
        cout << "No user logged in.\n";
        return false;
    }

    Book *b = books.get(bookId);
    if (!b) {
        cout << "Book not found.\n";
        return false;
    }

    if (!b->isOnLoan()) {
        cout << "Book is not on loan.\n";
        return false;
    }

    if (b->getLoan().username != user->getUsername()) {
        cout << "You are not the current borrower of this book.\n";
        return false;
    }

    b->endLoan();

    if (user_manager_) {
        UserManager::notify(user,
                            "You returned '" + b->getTitle() + "'. Thank you!");
    }

    return true;
}

bool BooksManager::extendLoan(User *user, const string &bookId, int extraDays) {
    if (!user) {
        cout << "No user logged in.\n";
        return false;
    }

    Book *b = books.get(bookId);
    if (!b) {
        cout << "Book not found.\n";
        return false;
    }

    const Loan &loan = b->getLoan();
    if (loan.username != user->getUsername()) {
        cout << "You are not the current borrower of this book.\n";
        return false;
    }

    // per-book waitlist for be implemented

    Day newDue{loan.due.value + extraDays};
    b->startLoan(loan.username, loan.start, newDue);

    if (user_manager_) {
        UserManager::notify(user,
                            "Loan for '" + b->getTitle() +
                            "' extended by " + to_string(extraDays) +
                            " days. New due day = " + to_string(newDue.value) + ".");
    }

    return true;
}


void BooksManager::listBooks() {
    books.forEach([](const string &id, const Book *b) {
        cout << "Book ID: " << id << ", Title: " << b->getTitle()
                << (b->isOnLoan() ? " (on loan)" : " (available)") << endl;
    });
}
