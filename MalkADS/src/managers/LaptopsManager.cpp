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


bool LaptopsManager::BorrowLaptop(const std::string &username) {
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
            tree->insert(startperiod, endperiod, username);
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

void LaptopsManager::loadLaptopBookingsFromFile() const {
    ifstream file("data/laptop_bookings.txt");
    if (!file) {
        // no bookings yet; fine
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        const size_t c1 = line.find(',');
        if (c1 == string::npos) continue;
        const size_t c2 = line.find(',', c1 + 1);
        if (c2 == string::npos) continue;
        const size_t c3 = line.find(',', c2 + 1);
        if (c3 == string::npos) continue;

        string id = line.substr(0, c1);
        string startStr = line.substr(c1 + 1, c2 - (c1 + 1));
        string endStr = line.substr(c2 + 1, c3 - (c2 + 1));
        string user = line.substr(c3 + 1);

        const int start = stoi(startStr);
        const int end = stoi(endStr);

        RedBlackIntervalTree **treePtr = laptopTable.get(id);
        if (!treePtr || !(*treePtr)) {
            // booking for laptop that no longer exists
            continue;
        }

        RedBlackIntervalTree *tree = *treePtr;
        tree->insert(start, end, user);
    }

    file.close();
}

void LaptopsManager::saveLaptopBookingsToFile() const {
    ofstream file("data/laptop_bookings.txt", ios::out | ios::trunc);
    if (!file) {
        cout << "Error opening laptop_bookings.txt for writing.\n";
        return;
    }

    const_cast<HashMap<string, RedBlackIntervalTree *> &>(laptopTable)
            .forEach([&](const string &id, RedBlackIntervalTree * &tree) {
                if (!tree) return;
                tree->forEachInterval([&](const int low, const int high, const std::string &username) {
                    file << id << "," << low << "," << high << "," << username << "\n";
                });
            });

    file.close();
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
