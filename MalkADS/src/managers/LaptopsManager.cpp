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


    int sDay, sMonth, sYear, sHour, sMinute;
    int eDay, eMonth, eYear, eHour, eMinute;

    cout << COLOR_PROMPT << "Enter start date and time: " <<  COLOR_RESET;
    long long startSec = getUserDateAsSeconds(sDay, sMonth, sYear, sHour, sMinute);

    cout << COLOR_PROMPT << "Enter end date and time: " << COLOR_RESET;
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
            printSuccess("Laptop " + id + " successfully booked.");

            cout << "Period: "
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
            << setw(2) << setfill('0') << eMinute;

            //cout << "  Period: [" << startperiod << ", " << endperiod << "]\n";
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
                u->addRoomBooking(low, high);
        });
    });
}

long long LaptopsManager::getUserDateAsSeconds(int &day, int &month, int &year, int &hour, int &minute) {

        // ---------- USER INPUT ----------
    string startDateStr, startTimeStr;
    cout << "Enter start date (dd/mm/yyyy): ";
    cin >> startDateStr;
    cout << "Enter start time (hh:mm): ";
    cin >> startTimeStr;


    // ---------- PARSE USER ENTERED DATA ----------
    parseDate(startDateStr, day, month, year);
    parseTime(startTimeStr, hour, minute);


    // ---------- CREATE tm STRUCT ----------
    tm userTime = {};
    userTime.tm_year = year - 1900;   // tm_year = years since 1900
    userTime.tm_mon  = month - 1;     // tm_mon  = 0–11
    userTime.tm_mday = day;
    userTime.tm_hour = hour;
    userTime.tm_min  = minute;
    userTime.tm_sec  = 0;

    // Convert user time → timestamp (seconds since 1970)
    time_t userStamp = mktime(&userTime);

    // ---------- REFERENCE DATE ----------
    tm ref = {};
    ref.tm_year = 2025 - 1900;
    ref.tm_mon  = 0;     // January
    ref.tm_mday = 1;     // 1st
    ref.tm_hour = 0;
    ref.tm_min  = 0;
    ref.tm_sec  = 0;

    time_t refStamp = mktime(&ref);

    // ---------- DIFF IN SECONDS ----------
    double diff = difftime(userStamp, refStamp);

    return static_cast<long long>(diff);
}



void LaptopsManager::parseDate(string stringdate, int &day, int &month, int &year) {
    // Find first and second slash
    int first = stringdate.find('/');
    int second = stringdate.find('/', first + 1);


    // Extract substrings
    string dayStr = stringdate.substr(0, first);
    string monthStr = stringdate.substr(first + 1, second - first - 1);
    string yearStr = stringdate.substr(second + 1);

    // Convert to integers
    day = stoi(dayStr);
    month = stoi(monthStr);
    year = stoi(yearStr);

}


void LaptopsManager::parseTime(string stringtime, int &hour, int &minute) {
    int colon = stringtime.find(':');

    string hourStr = stringtime.substr(0, colon);
    string minStr  = stringtime.substr(colon + 1);

    hour = stoi(hourStr);
    minute = stoi(minStr);

}

