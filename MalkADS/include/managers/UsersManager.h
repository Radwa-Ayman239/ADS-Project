#ifndef USERS_H
#define USERS_H

#include <string>
#include "../models/user.h"
#include "../structures/hash_map.h"
using namespace std;

class UsersManager {
private:
    //Data Structure Change
    HashMap<string, User> userTable;

public:
    UsersManager();

    void loadUsersFromFile(); //Reads user.txt, creates the user objects, adds them to the hashmap

    const User* login(const string &username, const string &password); //Checks entered username and password against hashmap
};

#endif
