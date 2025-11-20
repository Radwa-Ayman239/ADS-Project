#ifndef USER_H
#define USER_H
#include "../structures/IntervalTreeComplete.h"
using namespace std;

#include <string>

class User {
private:
    string username;
    string password;
    bool isAdmin;

    RedBlackIntervalTree roomBookings;
    RedBlackIntervalTree laptopBookings;

public:
    User();

    User(string uname, string pass, bool admin = false);

    string getPassword();

    bool getIsAdmin() const { return isAdmin; }

    const std::string &getUsername() const { return username; }

    bool canBookRoom(const int start, const int end) {
        return !roomBookings.searchOverlap(start, end, false);
    }

    void addRoomBooking(const int start, const int end) {
        roomBookings.insert(start, end, username);
    }

    bool canBookLaptop(const int start, const int end) {
        return !laptopBookings.searchOverlap(start, end, false);
    }

    void addLaptopBooking(const int start, const int end) {
        laptopBookings.insert(start, end, username);
    }
};

#endif
