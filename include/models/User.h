//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_USER_H
#define ADS_PROJECT_USER_H

#include "queue.h"
#include <string>
#include <utility>
using namespace std;

enum Role {
    ADMIN,
    NORMAL,
};

class User {
private:
    int id_;
    string username_;
    string password_;
    Role role_;
    bool isActive_;
    queue notifications;

    static int counter_;

public:
    explicit User(string username, string password) : id_(++counter_), username_(std::move(username)),
                                                      password_(std::move(password)), role_(Role::NORMAL),
                                                      isActive_(true) {
    }

    bool checkPassword(string inputPassword);

    bool changePassword(string oldPassword, string newPassword);

    void notify(string msg);

    queue allNotifications();

    bool isActive();

    bool isNormal();
};

int User::counter_ = 0;

#endif //ADS_PROJECT_USER_H
