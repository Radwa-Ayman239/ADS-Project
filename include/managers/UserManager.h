//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_USERMANAGER_H
#define ADS_PROJECT_USERMANAGER_H
#include "../queue.h"
#include "../hash_map.h"
#include "../models/User.h"

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

    User* signup();

    bool changePassword();

    void notify(User* u, const string& msg);

    void allNotifications(User* u);

    void showUsers();
};

#endif //ADS_PROJECT_USERMANAGER_H
