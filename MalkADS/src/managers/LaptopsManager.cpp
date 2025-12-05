#include "../../include/managers/LaptopsManager.h"
#include "../../include/helpers/UIHelpers.h"
#include "../../include/helpers/ResourceIO.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

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
    int sDay, sMonth, sYear, sHour, sMinute;
    int eDay, eMonth, eYear, eHour, eMinute;

    printSectionHeader("Borrow a Laptop");

    cout << "\nEnter start date and time: ";
    long long startSec = getUserDateAsSeconds(sDay, sMonth, sYear, sHour, sMinute);
    cout << "\nEnter end date and time: ";
    long long endSec = getUserDateAsSeconds(eDay, eMonth, eYear, eHour, eMinute);

    if (!user->canBookLaptop(startSec, endSec)) {
        printError("You already have a laptop booking during this period.");
        return false;
    }

    bool booked = false;
    cout << COLOR_MENU << "\nSearching for an available laptop...\n\n" << COLOR_RESET;

    laptopTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        if (booked) return;
        if (!tree->searchOverlap(startSec, endSec, false)) {
            tree->insert(startSec, endSec, user->getUsername());
            user->addLaptopBooking(startSec, endSec);
            
            cout << COLOR_PROMPT << "Laptop " << id << " successfully booked from: " << COLOR_RESET
                 << setw(2) << setfill('0') << sDay << "/"
                 << setw(2) << setfill('0') << sMonth << "/"
                 << sYear << " "
                 << setw(2) << setfill('0') << sHour << ":"
                 << setw(2) << setfill('0') << sMinute
                 << " to "
                 << setw(2) << setfill('0') << eDay << "/"
                 << setw(2) << setfill('0') << eMonth << "/"
                 << eYear << " "
                 << setw(2) << setfill('0') << eHour << ":"
                 << setw(2) << setfill('0') << eMinute << endl;

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

long long LaptopsManager::getUserDateAsSeconds(int &day, int &month, int &hour, int &year, int &minute) {
    string startDateStr, startTimeStr;
    cout << "Enter start date (dd/mm/yyyy): ";
    cin >> startDateStr;
    cout << "Enter start time (hh:mm): ";
    cin >> startTimeStr;
    
    parseDate(startDateStr, day, month, year);
    parseTime(startTimeStr, hour, minute);
    
    tm userTime = {};
    userTime.tm_year = year - 1900;
    userTime.tm_mon = month - 1;
    userTime.tm_mday = day;
    userTime.tm_hour = hour;
    userTime.tm_min = minute;
    userTime.tm_sec = 0;
    
    time_t userStamp = mktime(&userTime);
    
    tm ref = {};
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *std::localtime(&t_c);
    int currentYear = local_tm.tm_year + 1900;

    ref.tm_year = currentYear - 1900;
    ref.tm_mon = 0;
    ref.tm_mday = 1;
    ref.tm_hour = 0;
    ref.tm_min = 0;
    ref.tm_sec = 0;
    
    time_t refStamp = mktime(&ref);
    double diff = difftime(userStamp, refStamp);
    
    return static_cast<long long>(diff);
}

