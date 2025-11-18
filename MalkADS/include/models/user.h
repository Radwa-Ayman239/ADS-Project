#ifndef USER_H
#define USER_H
using namespace std;

#include <string>

class User {

private:
    string username;
    string password;

public:
    User();
    User(const string& uname, const string& pass);

    string getPassword();
};

#endif
