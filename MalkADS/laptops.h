#ifndef LAPTOPS_H
#define LAPTOPS_H

#include <string>
#include "IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class Laptops {
private:
//Data Structure Change
    unordered_map<string, RedBlackIntervalTree*> laptopTable;

public:
    Laptops();
    ~Laptops(); // delete all interval trees

    void loadLaptopsFromFile(); // read laptop IDs from "laptop.txt"

    bool BorrowLaptop();
};

#endif
