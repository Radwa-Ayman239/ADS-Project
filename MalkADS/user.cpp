#include "user.h"
using namespace std;

User::User() {
    username = "";
    password = "";
}
User::User(const string& uname, const string& pass) {
    username = uname;
    password = pass;
}

string User::getPassword() {
    return password;
}