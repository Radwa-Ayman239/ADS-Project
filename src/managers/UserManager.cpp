//
// Created by hamdy on 11/10/2025.
//

#include "managers/UserManager.h"
#include "models/User.h"
#include "hash_map.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>

#ifdef _WIN32
#include <direct.h>    // _mkdir
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>  // mkdir(path, mode_t)
#define MKDIR(path) mkdir(path, 0755)
#endif

using namespace std;

UserManager::UserManager() {
    // preload users from a file
    load();
}

UserManager::~UserManager() {
    save();

    users.forEach([](const string &key, const User *u) {
        delete u;
    });
    users.clear();
}


void UserManager::save() {
    if (MKDIR("../data") != 0) {
    }

    ofstream outFile("../data/users.csv");
    if (!outFile) {
        cout << "Failed to open file for saving.\n";
        return;
    }

    users.forEach([&outFile](const string &username, const User *userPtr) {
        if (userPtr != nullptr) {
            outFile << userPtr->getUsername() << ','
                    << userPtr->getPassword() << '\n';
            //<< (userPtr->isNormal() ? 1 : 0) << '\n'
            //<< (userPtr->isActive() ? 1 : 0) << '\n';
        }
    });
}


void UserManager::load() {
    ifstream inFile("../data/users.csv");
    if (!inFile) {
        cout << "No saved users found.\n";
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        const size_t commaPos = line.find(',');
        if (commaPos == string::npos) continue;

        string username = line.substr(0, commaPos);
        const string password = line.substr(commaPos + 1);

        if (username.empty()) continue;

        cout << "Loaded: " << username << "  " << password << endl;
        auto u = new User(username, password);
        if (!users.putNew(username, u)) delete u;
    }
}

User *UserManager::signup(const string &username, const string &password) {
    if (username.empty() || password.empty())
        return nullptr;

    if (users.contains(username))
        return nullptr;

    const auto u = new User(username, password);
    if (!users.putNew(username, u)) {
        delete u;
        return nullptr;
    }

    return u;
}


User *UserManager::signup() {
    cout << "\n--- Sign up ---\n";

    string username;
    while (true) {
        cout << "Enter username: ";
        getline(cin, username);

        if (username.empty()) {
            cout << "Username cannot be empty.\n";
            continue;
        }

        if (users.contains(username)) {
            cout << "Username already exists.\n";
            continue;
        }

        break;
    }

    string password;
    do {
        cout << "Enter password: ";
        getline(cin, password);
        if (password.empty()) cout << "Password cannot be empty.\n";
    } while (password.empty());

    User *u = signup(username, password);
    if (!u) {
        cout << "Failed to create user.\n";
        return nullptr;
    }

    cout << "User created. Your ID = " << u->getId() << "\n";
    return u;
}


User *UserManager::login() const {
    cout << "\n--- Login ---\n";
    string username, password;
    cout << "Username: ";
    getline(cin, username);

    constexpr int maxAttempts = 3;

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
        cout << "Password: ";
        getline(cin, password);

        User *u = login(username, password);
        if (u) {
            cout << "Welcome " << u->getUsername() << " (id=" << u->getId() << ")\n";
            return u;
        }

        cout << "Incorrect credentials. ";
        if (attempt < maxAttempts)
            cout << "Try again (" << (maxAttempts - attempt) << " attempts left).\n";
        else
            cout << "No attempts left. Login failed.\n";
    }

    return nullptr;
}

User *UserManager::login(const string &username, const string &password) const {
    User *u = users.get(username);
    if (!u) return nullptr;
    return u->checkPassword(password) ? u : nullptr;
}


/*bool UserManager::deactivateUser() {
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
}*/


/*bool UserManager::changePassword(User* user) {
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
}*/

void UserManager::notify(User *u, const string &msg) {
    if (u) u->notify(msg);
}

void UserManager::allNotifications(User *u) {
    if (!u) return;

    queue q = u->allNotifications();

    if (q.isEmpty()) {
        cout << "No notifications.";
        return;
    }

    cout << "--- Notifications for " << u->getUsername() << " ---\n";
    while (!q.isEmpty()) {
        cout << q.Front() << endl;
        q.dequeue();
    }
}


void UserManager::showUsers() {
    users.forEach([](const string &username, const User *userPtr) {
        if (userPtr != nullptr) {
            cout << "Username: " << username
                    // << "Active: " << (userPtr->isActive() ? "Yes" : "No")
                    //<< ", Role: " << (userPtr->isNormal() ? "Normal" : "Admin")
                    << endl;
        } else {
            cout << username << ": NULL User pointer\n";
        }
    });
}
