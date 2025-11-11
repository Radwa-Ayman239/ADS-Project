//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_USER_H
#define ADS_PROJECT_USER_H

#include "queue.h"
#include <string>
#include <utility>

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

    const string &getUsername() const { return username_; }
};

int User::counter_ = 0;

namespace std {
    template<>
    struct hash<User> {
        size_t operator()(const User &u) const noexcept {
            return std::hash<std::string>()(u.getUsername());
        }
    };
}

#endif //ADS_PROJECT_USER_H
