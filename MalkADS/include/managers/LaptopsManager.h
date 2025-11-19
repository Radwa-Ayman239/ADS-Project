#ifndef LAPTOPS_H
#define LAPTOPS_H

#include <string>
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
using namespace std;

class LaptopsManager {
private:
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> laptopTable;

    void loadLaptopsFromFile(); // read laptop IDs from "laptop.txt"
    void saveLaptopsToFile() const;

public:
    LaptopsManager();

    ~LaptopsManager(); // delete all interval trees

    bool BorrowLaptop();

    // admin operations
    void addLaptopInteractive();

    void removeLaptopInteractive();
};

#endif
