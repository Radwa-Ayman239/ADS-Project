//
// Created by hamdy on 11/10/2025.
//

#include "managers/UserManager.h"
#include "models/User.h"
#include "hash_map.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

UserManager::UserManager() {
    // pre-load users from a file
    load();

}


void UserManager::save() {
    ofstream outFile("users.txt");
    if (!outFile) {
        cout << "Failed to open file for saving.\n";
        return;
    }

    users.forEach([&outFile](const string& username, User* userPtr) {
        if (userPtr != nullptr) {
            outFile << userPtr->getUsername() << '\n'
                    << userPtr->getPassword() << '\n';
                    //<< (userPtr->isNormal() ? 1 : 0) << '\n'
                    //<< (userPtr->isActive() ? 1 : 0) << '\n';
        }
    });

    outFile.close();
}


void UserManager::load() {
    ifstream inFile("data/users.txt");
    if (!inFile) {
        cout << "No saved users found.\n";
        return;
    }

    string username, password;
    while (getline(inFile, username) && getline(inFile, password)) {
        User* u = new User(username, password);
        users.putNew(username, u);
    }
}



User* UserManager::signup() {
    cout << "\n--- Sign up ---\n";

    string username;
    do {
        cout << "Enter username: ";
        getline(cin, username);
        if (username.empty()) cout << "Username cannot be empty.\n";
        else if (users.contains(username)) cout << "Username already exists.\n";
    } while (username.empty() || users.contains(username));

    string password;
    do {
        cout << "Enter password: ";
        getline(cin, password);
        if (password.empty()) cout << "Password cannot be empty.\n";
    } while (password.empty());

    User* u = new User(username, password);
    users.putNew(username, u);

    cout << "User created. Your ID = " << u->getId() << "\n";
     save();
    return u;

}


User* UserManager::login() {
    cout << "\n--- Login ---\n";
    string username;
    cout << "Username: ";
    getline(cin, username);


    User* u = users.get(username);
    if (u == nullptr) {
        cout << "User not found.\n";
        return nullptr;
    }

    /*if (!u->isActive()) {
        cout << "Account is deactivated.\n";
        return nullptr;
    }*/

    const int maxAttempts = 3;
    string password;
    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
        cout << "Password: ";
        getline(cin, password);

        if (u->checkPassword(password)) {
            cout << "Welcome " << u->getUsername() << " (id=" << u->getId() << ")\n";
            return u;
        } else {
            cout << "Incorrect password. ";
            if (attempt < maxAttempts)
                cout << "Try again (" << (maxAttempts - attempt) << " attempts left).\n";
            else
                cout << "No attempts left. Login failed.\n";
        }
    }
    return nullptr;

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

void UserManager::notify(User* u, const string& msg) {
    if (u) u->notify(msg);
}

void UserManager::allNotifications(User* u) {
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
    users.forEach([](const string& username, User* userPtr) {
        if (userPtr != nullptr) {
            cout << "Username: " << username << ", "
           // << "Active: " << (userPtr->isActive() ? "Yes" : "No")
            //<< ", Role: " << (userPtr->isNormal() ? "Normal" : "Admin")
            << endl;
        } else {
            cout << username << ": NULL User pointer\n";
        }
    });
}



