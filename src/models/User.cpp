//
// Created by hamdy on 11/10/2025.
//
#include "models/User.h"

bool User::checkPassword(string inputPassword) {


}

bool User::changePassword(string oldPassword, string newPassword) {

}

void User::notify(string msg) {

    notifications.enqueue(std::move(msg));
}

queue User::allNotifications() {


    return notifications;
}
void clearNotifications();


//Done
bool User::isActive() {
    return isActive_;
}
//Done
bool User::isNormal() {
    return role_ == Role::NORMAL;
}



