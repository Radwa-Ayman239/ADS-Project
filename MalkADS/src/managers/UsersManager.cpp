#include "../../include/managers/UsersManager.h"
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
        int commaPos = line.find(',');
        if (commaPos != string::npos) {
            string uname = line.substr(0, commaPos);
            string pass = line.substr(commaPos + 1);
            User newUser(uname, pass);
            //Data Structure Change
            userTable[uname] = newUser;
        }
    }

    file.close();
}

//Check login using unordered_map
bool UsersManager::login(const string& uname, const string& pass) const {
    
    //Checks if username actually exists

    //Data Structure Change
    if (userTable.find(uname) == userTable.end() ) return false;

    //If it does, checks if password entered for this username is correct.
    
    //Data Structure Change
    User finduser = userTable.at(uname);
    return (finduser.getPassword() == pass);
}