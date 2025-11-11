//
// Created by hamdy on 11/10/2025.
//

#include "managers/UserManager.h"
#include "models/User.h"
#include <iostream>
#include <string>
using namespace std;
bool UserManager::signup() {
    cout << "\n--- Sign up ---\n";
    string username;
    cout << "Enter username: ";
    getline(cin, username);

    if (username.empty()) {
        cout << "Username cannot be empty.\n";
        return false;
    }

    if (users.contains(username)) {
        cout << "Username already exists.\n";
        return false;
    }

    string idStr;
    int id;
    while (true) {
        cout << "Enter your ID (integer): ";
        getline(cin, idStr);
        try {
            id = std::stoi(idStr);
            break;
        } catch (...) {
            cout << "Invalid ID. Please enter a number.\n";
        }
    }

    string password;
    cout << "Enter password: ";
    getline(cin, password);
    if (password.empty()) {
        cout << "Password cannot be empty.\n";
        return false;
    }

    User* u = new User(id, username, password, Role::NORMAL);
    users.insert(username, u);

    cout << "User created. Your ID = " << u->getId() << "\n";
    // save();
    return true;
}
User* UserManager::login() {
    cout << "\n--- Login ---\n";
    string username;
    cout << "Username: ";
    getline(cin, username);
    string password;
    cout << "Password: ";
    getline(cin, password);

    User* u = nullptr;
    if (!users.get(username, u)) {
        cout << "User not found.\n";
        return nullptr;
    }

    if (!u->isActive()) {
        cout << "Account is deactivated.\n";
        return nullptr;
    }

    if (!u->checkPassword(password)) {
        cout << "Incorrect password.\n";
        return nullptr;
    }

    cout << "Welcome " << u->getUsername() << " (id=" << u->getId() << ")\n";
    return u;
}
User* UserManager::findUser(const string &username) {
    User* u = nullptr;
    if (users.get(username, u)) return u;
    return nullptr;
}
bool UserManager::deactivateUser() {
    cout << "\n--- Deactivate user ---\n";
    string username;
    cout << "Username to deactivate: ";
    getline(cin, username);
    User* u = findUser(username);
    if (!u) {
        cout << "User not found.\n";
        return false;
    }
    if (!u->isNormal()) {
        cout << "Cannot deactivate an admin.\n";
        return false;
    }
    u->setActive(false);
    cout << "User " << username << " deactivated.\n";
    // save();
    return true;
}
bool UserManager::changePassword(User* user) {
    if (user == nullptr) return false;
    cout << "\n--- Change password for " << user->getUsername() << " ---\n";
    string oldp, newp;
    cout << "Old password: ";
    getline(cin, oldp);
    cout << "New password: ";
    getline(cin, newp);
    if (user->changePassword(oldp, newp)) {
        cout << "Password changed.\n";
        // save();
        return true;
    } else {
        cout << "Old password incorrect.\n";
        return false;
    }
}

void UserManager::showUsers() {
    users.forEach([](const string& username, User* userPtr) {
        if (userPtr != nullptr) {
            cout << "Username: " << username << ", "
            << "Active: " << (userPtr->isActive() ? "Yes" : "No")
            << ", Role: " << (userPtr->isNormal() ? "Normal" : "Admin")
            << endl;
        } else {
            cout << username << ": NULL User pointer\n";
        }
    });
}

