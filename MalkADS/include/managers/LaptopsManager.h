#ifndef LAPTOPS_H
#define LAPTOPS_H

#include <string>

#include "UsersManager.h"
#include "../models/user.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
#include "../helpers/ResourceIO.h"
using namespace std;

class LaptopsManager {
private:
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> laptopTable;

    void loadLaptopsFromFile(); // read laptop IDs from "laptop.txt"
    void saveLaptopsToFile() const;

    void loadLaptopBookingsFromFile() const;

    void saveLaptopBookingsToFile() const;

public:
    LaptopsManager();

    ~LaptopsManager(); // delete all interval trees

    bool BorrowLaptop(User *user);

    // admin operations
    void addLaptopInteractive();

    void removeLaptopInteractive();

    void showUserBookings(const std::string &username) const;

    void syncUserBookings(UsersManager &usersManager);

    void parseDate(string stringdate, int &day, int &month, int &year);
    void parseTime(string stringdate, int &hour, int &minute);

    // Converts a date/time into seconds since Jan 1 2025, 00:00
    long long getUserDateAsSeconds(int &day, int &month, int &year, int &hour, int &minute);
};

#endif
