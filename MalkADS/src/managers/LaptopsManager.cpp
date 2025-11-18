#include "../../include/managers/LaptopsManager.h"
#include <fstream>
#include <iostream>

LaptopsManager::LaptopsManager() {}

LaptopsManager::~LaptopsManager() {
    //Data Structure Change
    unordered_map<string, RedBlackIntervalTree*>::iterator it;

    //Data Structure Change
    for (it = laptopTable.begin(); it != laptopTable.end(); ++it) {
        delete it->second; 
    }

    laptopTable.clear();
}


void LaptopsManager::loadLaptopsFromFile() {
    ifstream file("data\\laptops.txt");
    if (!file) {
        cout << "Error opening laptop.txt\n";
        return;
    }

    string laptopID;
    while (getline(file, laptopID)) {
        RedBlackIntervalTree* tree = new RedBlackIntervalTree();
        //Data Structure Change
        laptopTable[laptopID] = tree;
    }
    
}


bool LaptopsManager::BorrowLaptop() {
    int startperiod, endperiod;
    cout << "\n\nEnter your desired borrowing period";
    cout << "\nStart of borrowing period: "; cin >> startperiod;
    cout << "End of borrowing period: "; cin >> endperiod;

    // Iterate over all laptops in the map
    //Data Structure Change
    unordered_map<string, RedBlackIntervalTree*>::iterator it;
    for (it = laptopTable.begin(); it != laptopTable.end(); ++it) {
        RedBlackIntervalTree* tree = it->second;

        // Check if the interval is free
        if (!tree->searchOverlap(startperiod, endperiod, false)) {
            tree->insert(startperiod, endperiod);
            cout << "Laptop " << it->first << " successfully booked from " << startperiod << " to " << endperiod << "!\n";
            return true; // booking done, exit function
        }
    }

    // If we reach here, all laptops had conflicts
    cout << "No laptops are available for the requested period.\n";
    return false;
}
