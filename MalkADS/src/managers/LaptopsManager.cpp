#include "../../include/managers/LaptopsManager.h"
#include <fstream>
#include <iostream>

LaptopsManager::LaptopsManager() {
    loadLaptopsFromFile();
    loadLaptopBookingsFromFile();
}

LaptopsManager::~LaptopsManager() {
    saveLaptopsToFile();
    saveLaptopBookingsToFile();
    //Data Structure Change
    laptopTable.forEach([](const string &id, RedBlackIntervalTree * &tree) {
        delete tree;
        tree = nullptr;
    });
    laptopTable.clear();
}


void LaptopsManager::loadLaptopsFromFile() {
    loadResourceIDsFromFile("data/laptops.txt", laptopTable);
}


bool LaptopsManager::BorrowLaptop(User *user) {
    int startperiod, endperiod;
    cout << "\n\nEnter your desired borrowing period";
    cout << "\nStart of borrowing period: ";
    cin >> startperiod;
    cout << "End of borrowing period: ";
    cin >> endperiod;

    if (!user->canBookLaptop(startperiod, endperiod)) {
        cout << "\nYou already have a laptop booking during this period.\n";
        return false;
    }

    // Iterate over all laptops in the map
    //Data Structure Change
    bool booked = false;
    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        if (booked) return;

        if (!tree->searchOverlap(startperiod, endperiod, false)) {
            tree->insert(startperiod, endperiod, user->getUsername());
            user->addLaptopBooking(startperiod, endperiod);
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
    saveResourceIDsToFile("data/laptops.txt", laptopTable);
}

void LaptopsManager::loadLaptopBookingsFromFile() const {
    loadBookingsFromFile("data/laptop_bookings.txt",
                         const_cast<HashMap<string, RedBlackIntervalTree *> &>(laptopTable));
}

void LaptopsManager::saveLaptopBookingsToFile() const {
    saveBookingsToFile("data/laptop_bookings.txt", laptopTable);
}

void LaptopsManager::showUserBookings(const std::string &username) const {
    bool any = false;

    const_cast<HashMap<string, RedBlackIntervalTree *> &>(laptopTable).forEach(
        [&](const string &id, RedBlackIntervalTree * &tree) {
            if (!tree) return;

            tree->forEachInterval([&](const int low, const int high, const string &user) {
                if (user == username) {
                    cout << "- Laptop " << id
                            << " | Period: [" << low << ", " << high << "]\n";
                    any = true;
                }
            });
        });

    if (!any)
        cout << "You have no laptop bookings.\n";
}
