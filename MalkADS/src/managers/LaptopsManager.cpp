#include "../../include/managers/LaptopsManager.h"
#include <fstream>
#include <iostream>

LaptopsManager::LaptopsManager() {
    loadLaptopsFromFile();
}

LaptopsManager::~LaptopsManager() {
    saveLaptopsToFile();
    //Data Structure Change
    laptopTable.forEach([](const string &id, RedBlackIntervalTree * &tree) {
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
        auto *tree = new RedBlackIntervalTree();
        //Data Structure Change
        laptopTable.putNew(laptopID, tree);
    }
}


bool LaptopsManager::BorrowLaptop() {
    int startperiod, endperiod;
    cout << "\n\nEnter your desired borrowing period";
    cout << "\nStart of borrowing period: ";
    cin >> startperiod;
    cout << "End of borrowing period: ";
    cin >> endperiod;

    // Iterate over all laptops in the map
    //Data Structure Change
    bool booked = false;
    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
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

void LaptopsManager::addLaptopInteractive() {
    string id;
    cout << "\nEnter new laptop ID: ";
    cin >> id;

    if (laptopTable.contains(id)) {
        cout << "A laptop with this ID already exists.\n";
        return;
    }

    auto *tree = new RedBlackIntervalTree();
    if (!laptopTable.putNew(id, tree)) {
        delete tree;
        cout << "Failed to add laptop.\n";
        return;
    }

    cout << "Laptop " << id << " added successfully.\n";
}

void LaptopsManager::removeLaptopInteractive() {
    string id;
    cout << "\nEnter laptop ID to remove: ";
    cin >> id;

    RedBlackIntervalTree **treePtr = laptopTable.get(id);
    if (!treePtr || !(*treePtr)) {
        cout << "No laptop with this ID.\n";
        return;
    }

    delete *treePtr;
    *treePtr = nullptr;

    laptopTable.erase(id);
    cout << "Laptop " << id << " removed.\n";
}

void LaptopsManager::saveLaptopsToFile() const {
    ofstream file("data/laptops.txt", ios::out | ios::trunc);
    if (!file) {
        cout << "Error opening laptops.txt for writing.\n";
        return;
    }

    const_cast<HashMap<string, RedBlackIntervalTree *> &>(laptopTable).forEach(
        [&](const string &id, RedBlackIntervalTree * &tree) {
            file << id << "\n";
        });

    file.close();
}
