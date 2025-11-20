#ifndef LAPTOPS_H
#define LAPTOPS_H

#include <string>

#include "../models/user.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
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

    bool BorrowLaptop(User* user);

    // admin operations
    void addLaptopInteractive();

    void removeLaptopInteractive();

    void showUserBookings(const std::string& username) const;
};

#endif
