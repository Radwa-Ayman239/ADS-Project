#include "../../include/managers/LaptopsManager.h"
#include <fstream>
#include <iostream>

LaptopsManager::LaptopsManager() {}

LaptopsManager::~LaptopsManager() {
    //Data Structure Change
    laptopTable.forEach([](const string& id, RedBlackIntervalTree* &tree) {
        delete tree;
        tree = nullptr;
    });
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
        auto* tree = new RedBlackIntervalTree();
        //Data Structure Change
        laptopTable.putNew(laptopID, tree);
    }
    
}


bool LaptopsManager::BorrowLaptop() {
    int startperiod, endperiod;
    cout << "\n\nEnter your desired borrowing period";
    cout << "\nStart of borrowing period: "; cin >> startperiod;
    cout << "End of borrowing period: "; cin >> endperiod;

    // Iterate over all laptops in the map
    //Data Structure Change
    bool booked = false;
    laptopTable.forEach([&](const string &id, RedBlackIntervalTree* &tree) {
        if (booked) return;

        if (!tree->searchOverlap(startperiod, endperiod, false)) {
            tree->insert(startperiod, endperiod);
            cout << "Laptop " << id << " successfully booked from " << startperiod << " to " << endperiod << "!\n";
            booked = true;
        }
    });

    // If we reach here, all laptops had conflicts
    if (!booked) {
        cout << "No laptops available for the requested period.\n";
    }
    return booked;
}
