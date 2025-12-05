#ifndef USERS_H
#define USERS_H

#include "../models/user.h"
#include "../structures/hash_map.h"
#include <string>

using namespace std;

class UsersManager {
private:
  // Data Structure Change
  HashMap<string, User> userTable;

public:
  UsersManager();

  void loadUsersFromFile(); // Reads user.txt, creates the user objects, adds
                            // them to the hashmap

  User *login(const string &username, const string &password);

  User *getUser(const string &uname) {
    if (!userTable.contains(uname))
      return nullptr;
    return userTable.get(uname);
  }

  // Checks entered username and password against hashmap
};

#endif
