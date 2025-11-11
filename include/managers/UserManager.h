//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_USERMANAGER_H
#define ADS_PROJECT_USERMANAGER_H
#include "hash_map.h"
#include "models/User.h"
#include <string>
using namespace std;

class UserManager {
private:
    HashMap<string, User *> users;

    void load();

    void save();

public:
    UserManager();

    User* login();

    bool signup();

    bool changePassword();

    void notify();

    void allNotifications();

    void showUsers();
};

#endif //ADS_PROJECT_USERMANAGER_H
