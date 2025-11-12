//
// Created by hamdy on 11/10/2025.
//
#include "User.h"
#include "../queue.h"


using namespace std;

bool User::checkPassword(const string& inputPassword) const {
    return inputPassword == password_;
}

/*
bool User::changePassword(string oldPassword, string newPassword) {
}
*/

void User::notify(string msg) {
    notifications.enqueue(move(msg));
}



queue User::allNotifications() {
    queue temp = notifications;
    notifications.clear();    //cuz accorrding to the logic, we won't display all the history all the time
    return temp;
}




/*
bool User::isActive() {
    return isActive_;
}


bool User::isNormal() {
    return role_ == Role::NORMAL;
}
*/


