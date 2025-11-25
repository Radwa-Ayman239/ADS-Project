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
    
    void loadLaptopBookingsFromFile() const;

public:
    LaptopsManager();

    ~LaptopsManager(); // delete all interval trees

    bool BorrowLaptop(User *user);
    
    // Non-interactive version for Python API
    // Non-interactive version for Python API
    bool borrowLaptopDirect(User *user, const string& laptopId, int startTime, int endTime);
    
    // Auto-assign available laptop
    string borrowAnyLaptopDirect(User *user, int startTime, int endTime);

    // admin operations
    void addLaptopInteractive();
    
    // Non-interactive version for Python API
    bool addLaptopDirect(const string& laptopId);

    void removeLaptopInteractive();
    
    // Non-interactive version for Python API
    bool removeLaptopDirect(const string& laptopId);
    
    // Save methods - made public for Python access
    void saveLaptopsToFile() const;
    void saveLaptopBookingsToFile() const;

    void showUserBookings(const std::string &username) const;

    void syncUserBookings(UsersManager &usersManager);
    
    // Iterator for Python bindings
    template<typename Func>
    void forEachBooking(Func func) {
        laptopTable.forEach([&](const string& laptopId, RedBlackIntervalTree*& tree) {
            if (!tree) return;
            tree->forEachInterval([&](int low, int high, const string& username) {
                func(laptopId, low, high, username);
            });
        });
    }

    // Get list of all laptop IDs
    template<typename Func>
    void forEachLaptop(Func func) {
        laptopTable.forEach([&](const string& laptopId, RedBlackIntervalTree*& tree) {
            func(laptopId);
        });
    }
};

#endif
