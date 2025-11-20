#include "../../include/managers/UsersManager.h"
#include "../../include/helpers/UIHelpers.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

UsersManager::UsersManager() {
}

// Load users from file into the hashmap
void UsersManager::loadUsersFromFile() {
    ifstream file(R"(data\users.txt)");
    if (!file) {
        cout << "Unable to open file: users.txt" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        int c1 = line.find(',');
        if (c1 == string::npos) continue;
        int c2 = line.find(',', c1 + 1);

        string uname = line.substr(0, c1);
        string pass;
        bool adminFlag = false;

        if (c2 == string::npos) {
            pass = line.substr(c1 + 1);
        } else {
            pass = line.substr(c1 + 1, c2 - (c1 + 1));
            string flagStr = line.substr(c2 + 1);
            adminFlag = (flagStr == "1" || flagStr == "true" || flagStr == "True");
        }

        User newUser(uname, pass, adminFlag);
        userTable.putNew(uname, newUser);
    }

    file.close();
}

//Check login using unordered_map
User *UsersManager::login(const string &uname, const string &pass) {
    //Checks if username actually exists

    //Data Structure Change
    if (!userTable.contains(uname)) return nullptr;

    User *u = userTable.get(uname);
    if (!u) return nullptr;

    if (u->getPassword() != pass) return nullptr;
    return u;
}
