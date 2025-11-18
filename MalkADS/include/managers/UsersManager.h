#ifndef USERS_H
#define USERS_H

#include <string>
#include "../models/user.h"
#include <unordered_map>
using namespace std;

class UsersManager {
private:
    //Data Structure Change
    unordered_map<string, User> userTable;

public:
    UsersManager();

    void loadUsersFromFile(); //Reads user.txt, creates the user objects, adds them to the hashmap

    bool login(const string& username, const string& password) const; //Checks entered username and password against hashmap


};

#endif
