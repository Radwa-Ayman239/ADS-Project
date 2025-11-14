//
// Created by hamdy on 11/10/2025.
//

#include "managers/LibrarySystem.h"
#include <iostream>
using namespace std;

LibrarySystem::LibrarySystem() : user_manager_(), currentUser(nullptr) {
}

void LibrarySystem::showMenu() {
    cout << "\n=== Library System ===\n"
            << "1) Sign up\n"
            << "2) Login\n"
            << "3) Notify me (enqueue test notification)\n"
            << "4) Show my notifications\n"
            << "5) Show all users\n"
            << "0) Exit\n"
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

        cout << "Notification message: ";
        string msg;
        getline(cin, msg);
        UserManager::notify(currentUser, msg);
        cout << "Notification added.\n";
    } else if (choice == "4") {
        if (!currentUser) {
            cout << "Please login first.\n";
            return;
        }
        UserManager::allNotifications(currentUser);
    } else if (choice == "5") {
        user_manager_.showUsers();
    } else if (choice == "0") {
        // handled in run()
    } else {
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

