#include <utility>

#include "../../include/models/user.h"
using namespace std;

User::User() : username(), password(), isAdmin() {
}
User::User(string  uname, string  pass, const bool admin): username(std::move(uname)), password(std::move(pass)), isAdmin(admin) {
}

string User::getPassword() {
    return password;
}