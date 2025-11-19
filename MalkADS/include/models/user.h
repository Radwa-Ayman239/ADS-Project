#ifndef USER_H
#define USER_H
using namespace std;

#include <string>

class User {
private:
    string username;
    string password;
    bool isAdmin;

public:
    User();

    User(string uname, string pass, bool admin = false);

    string getPassword();

    bool getIsAdmin() const { return isAdmin; }
};

#endif
