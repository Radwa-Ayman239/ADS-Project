#ifndef LAPTOPS_H
#define LAPTOPS_H

#include <string>
#include "../structures/IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class LaptopsManager {
private:
//Data Structure Change
    unordered_map<string, RedBlackIntervalTree*> laptopTable;

public:
    LaptopsManager();
    ~LaptopsManager(); // delete all interval trees

    void loadLaptopsFromFile(); // read laptop IDs from "laptop.txt"

    bool BorrowLaptop();
};

#endif
