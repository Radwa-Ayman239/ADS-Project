//
// Created by hamdy on 11/10/2025.
//

#include "managers/UserManager.h"
#include "models/User.h"
#include <iostream>
#include <string>
using namespace std;

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

