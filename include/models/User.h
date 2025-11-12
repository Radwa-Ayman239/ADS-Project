//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_USER_H
#define ADS_PROJECT_USER_H

#include "../queue.h"
#include <string>
#include <utility>
using namespace std;


//lets forget about the admin now, maybe we don't need at the moment. lets complete the user first
/*enum Role {
    ADMIN,
    NORMAL,
};*/

class User {
private:
    int id_;
    string username_;
    string password_;
    //Role role_;
   // bool isActive_;
    queue notifications;

    static int counter_;

public:
//Sedra: move "moves" the input string into the member to avoid copying, effecient transfer.

    explicit User(string username, string password) : id_(++counter_), username_(move(username)),
                                                      password_(move(password)) /*role_(Role::NORMAL),*/
                                                     /* isActive_(true)*/ {


    }

    bool checkPassword(const string& inputPassword) const;

   // bool changePassword(string oldPassword, string newPassword);

    void notify(string msg);

    queue allNotifications();

    //bool isActive();

    //bool isNormal();

    const string &getUsername() const { return username_; }
    const int &getId() const { return id_; }
    const string &getPassword() const { return password_; }
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
