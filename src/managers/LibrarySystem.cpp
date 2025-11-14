//
// Created by hamdy on 11/10/2025.
//

#include "managers/LibrarySystem.h"
#include <iostream>
using namespace std;

LibrarySystem::LibrarySystem() : user_manager_(), books_manager_(&user_manager_), currentUser(nullptr) {
}

void LibrarySystem::showMenu() {
    cout << "\n=== Library System ===\n"
            << "1) Sign up\n"
            << "2) Login\n"
            // << "3) Notify me (enqueue test notification)\n"
            // << "4) Show my notifications\n"
            // << "5) Show all users\n"
            // << "0) Exit\n"
            << "3) Borrow Book\n"
            << "4) Return Book\n"
            << "5) List all Books\n"
            << "6) Add Book (manual)\n"
            << "7) Show my notifications\n"
            << "\n\nChoice: ";
}

void LibrarySystem::handleChoice(const string &choice) {
    if (choice == "1") {
        currentUser = user_manager_.signup();
    } else if (choice == "2") {
        currentUser = user_manager_.login();
    } else if (choice == "3") {
        if (!currentUser) {
            cout << "Please login first.\n";
            return;
        }

        cout << "Book ID to borrow: ";
        string bookId;
        getline(cin, bookId);
        books_manager_.borrowNow(currentUser, bookId, 1);
    } else if (choice == "4") {
        if (!currentUser) {
            cout << "Please login first.\n";
            return;
        }
        std::cout << "Book ID to return: ";
        std::string bookId;
        std::getline(std::cin, bookId);
        books_manager_.returnBook(currentUser, bookId, 1);
    } else if (choice == "5") {
        cout << "Under construction.\n";
    } else if (choice == "6") {
        string id, title;
        cout << "Book ID: "; getline(cin, id);
        cout << "Title: "; getline(cin, title);
        books_manager_.addBook(id, title);
    } else if (choice == "7") {
        if (!currentUser) {
            cout << "Log in first.\n";
            return;
        }
        UserManager::allNotifications(currentUser);
    }
    else {
        cout << "Invalid choice.\n";
    }
}

void LibrarySystem::run() {
    string choice;
    while (true) {
        showMenu();
        if (!getline(cin, choice)) {
            break;
        }

        if (choice == "0") {
            break;
        }

        handleChoice(choice);
    }
}
