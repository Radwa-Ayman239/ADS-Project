#include "../../include/managers/LaptopsManager.h"
#include "../../include/helpers/UIHelpers.h"
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
    printSectionHeader("Borrow a Laptop");

    int startperiod, endperiod;
    cout << COLOR_PROMPT << "Start of borrowing period: " << COLOR_RESET;
    cin >> startperiod;
    cout << COLOR_PROMPT << "End of borrowing period: " << COLOR_RESET;
    cin >> endperiod;

    if (!user->canBookLaptop(startperiod, endperiod)) {
        printError("You already have a laptop booking during this period.");
        return false;
    }

    bool booked = false;
    cout << COLOR_MENU << "\nSearching for an available laptop...\n\n" << COLOR_RESET;

    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        if (booked) return;
        if (!tree->searchOverlap(startperiod, endperiod, false)) {
            tree->insert(startperiod, endperiod, user->getUsername());
            user->addLaptopBooking(startperiod, endperiod);
            printSuccess("Laptop " + id + " successfully booked.");
            cout << "  Period: [" << startperiod << ", " << endperiod << "]\n";
            booked = true;
        }
    });

    if (!booked) {
        printError("No laptops available for the requested period.");
    } else {
        printHint("Collect your laptop from the help desk at the start of the booking period.");
    }
    return booked;
}

// Non-interactive version for Python API
bool LaptopsManager::borrowLaptopDirect(User *user, const string& laptopId, int startTime, int endTime) {
    if (!user) return false;
    
    // Check if user already has a conflicting booking
    if (!user->canBookLaptop(startTime, endTime)) {
        return false; // User conflict
    }
    
    RedBlackIntervalTree **treePtr = laptopTable.get(laptopId);
    if (!treePtr || !(*treePtr)) {
        return false; // Laptop doesn't exist
    }
    
    RedBlackIntervalTree *tree = *treePtr;
    
    // Check if laptop is available
    if (tree->searchOverlap(startTime, endTime, false)) {
        return false; // Laptop conflict
    }
    
    // Book the laptop
    tree->insert(startTime, endTime, user->getUsername());
    user->addLaptopBooking(startTime, endTime);
    
    return true;
}

// Auto-assign available laptop
string LaptopsManager::borrowAnyLaptopDirect(User *user, int startTime, int endTime) {
    if (!user) return "";
    
    // Check if user already has a conflicting booking
    if (!user->canBookLaptop(startTime, endTime)) {
        return ""; // User conflict
    }
    
    string bookedId = "";
    
    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        if (!bookedId.empty()) return;
        
        if (!tree->searchOverlap(startTime, endTime, false)) {
            tree->insert(startTime, endTime, user->getUsername());
            user->addLaptopBooking(startTime, endTime);
            bookedId = id;
        }
    });
    
    return bookedId;
}

void LaptopsManager::addLaptopInteractive() {
    printSectionHeader("Add Laptop");

    string id;
    cout << COLOR_PROMPT << "Enter new laptop ID: " << COLOR_RESET;
    cin >> id;

    if (laptopTable.contains(id)) {
        printError("A laptop with this ID already exists.");
        return;
    }

    auto *tree = new RedBlackIntervalTree();
    if (!laptopTable.putNew(id, tree)) {
        delete tree;
        printError("Failed to add laptop.");
        return;
    }

    printSuccess("Laptop " + id + " added successfully.");
}

// Non-interactive version for Python API
bool LaptopsManager::addLaptopDirect(const string& laptopId) {
    if (laptopTable.contains(laptopId)) {
        return false; // Laptop already exists
    }
    
    auto *tree = new RedBlackIntervalTree();
    if (!laptopTable.putNew(laptopId, tree)) {
        delete tree;
        return false;
    }
    
    return true;
}

void LaptopsManager::removeLaptopInteractive() {
    printSectionHeader("Remove Laptop");

    string id;
    cout << COLOR_PROMPT << "Enter laptop ID to remove: " << COLOR_RESET;
    cin >> id;

    RedBlackIntervalTree **treePtr = laptopTable.get(id);
    if (!treePtr || !(*treePtr)) {
        printError("No laptop with this ID.");
        return;
    }

    delete *treePtr;
    *treePtr = nullptr;
    laptopTable.erase(id);

    printSuccess("Laptop " + id + " removed.");
}

// Non-interactive version for Python API
bool LaptopsManager::removeLaptopDirect(const string& laptopId) {
    RedBlackIntervalTree **treePtr = laptopTable.get(laptopId);
    if (!treePtr || !(*treePtr)) {
        return false; // Laptop doesn't exist
    }
    
    delete *treePtr;
    *treePtr = nullptr;
    laptopTable.erase(laptopId);
    
    return true;
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
    cout << COLOR_MENU << "\nYour laptop bookings:\n\n" << COLOR_RESET;

    bool any = false;
    const_cast<HashMap<string, RedBlackIntervalTree *> &>(laptopTable).forEach(
        [&](const string &id, RedBlackIntervalTree * &tree) {
            if (!tree) return;
            tree->forEachInterval([&](const int low, const int high, const string &user) {
                if (user == username) {
                    cout << "  - Laptop " << id
                            << " | Period: [" << low << ", " << high << "]\n";
                    any = true;
                }
            });
        });

    if (!any) printHint("You have no laptop bookings.");
}

void LaptopsManager::syncUserBookings(UsersManager &usersManager) {
    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        if (!tree) return;
        tree->forEachInterval([&](const int low, const int high, const string &username) {
            User *u = usersManager.getUser(username);
            if (u)
                u->addLaptopBooking(low, high);
        });
    });
}
