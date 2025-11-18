#ifndef USERS_H
#define USERS_H

#include <string>
#include "user.h"
#include <unordered_map>
using namespace std;

class Users {
private:
    //Data Structure Change
    unordered_map<string, User> userTable;

public:
    Users();

    void loadUsersFromFile(); //Reads user.txt, creates the user objects, adds them to the hashmap

    bool login(const string& username, const string& password) const; //Checks entered username and password against hashmap


};

#endif
