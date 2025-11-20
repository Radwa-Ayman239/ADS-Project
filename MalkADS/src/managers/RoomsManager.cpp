#include "../../include/managers/RoomsManager.h"
#include "../../include/helpers/UIHelpers.h"
#include <fstream>
#include <iostream>

RoomsManager::RoomsManager() {
    loadRoomsFromFile();
    loadRoomBookingsFromFile();
}

RoomsManager::~RoomsManager() {
    saveRoomsToFile();
    saveRoomBookingsToFile();
    //Data Structure Change
    roomTable.forEach([](const string &id, RedBlackIntervalTree * &tree) {
        delete tree;
        tree = nullptr;
    });
    roomTable.clear();
}


void RoomsManager::loadRoomsFromFile() {
    loadResourceIDsFromFile("data/rooms.txt", roomTable);
}


bool RoomsManager::bookRoom(User *user) {
    printSectionHeader("Book a Study Room"); // NEW

    cout << COLOR_MENU << "\nAvailable rooms (with Available Time slots):\n\n" << COLOR_RESET;
    showRoomsWithAvailableTimes(0, 23);

    string roomchoice;

    cout << "\n" << COLOR_PROMPT << "Enter Room ID" << COLOR_RESET << ": ";
    cin >> roomchoice;
    cout << "\n";

    RedBlackIntervalTree **treePtr = roomTable.get(roomchoice);
    if (!treePtr || !(*treePtr)) {
        printError("No such room.");
        return false;
    }

    RedBlackIntervalTree *tree = *treePtr;

    int startperiod;
    int endperiod;
    cout << COLOR_PROMPT << "\nStart of booking period: " << COLOR_RESET;
    cin >> startperiod;
    cout << COLOR_PROMPT << "End of booking period: " << COLOR_RESET;
    cin >> endperiod;

    if (!user->canBookRoom(startperiod, endperiod)) {
        printError("You already have another room booked during this period.");
        return false;
    }

    if (tree->searchOverlap(startperiod, endperiod, true)) {
        printError("Unable to book room - conflict in scheduling.");
        return false;
    }

    tree->insert(startperiod, endperiod, user->getUsername());
    user->addRoomBooking(startperiod, endperiod);

    printSuccess("Room booked successfully!");
    printHint("Go to the library help desk at the start of booking period with your ID.");
    return true;
}

void RoomsManager::saveRoomsToFile() const {
    saveResourceIDsToFile("data/rooms.txt", roomTable);
}

void RoomsManager::addRoomInteractive() {
    printSectionHeader("Add New Room");

    string id;
    cout << COLOR_PROMPT << "Enter new room ID: " << COLOR_RESET;
    cin >> id;

    if (roomTable.contains(id)) {
        printError("A room with this ID already exists.");
        return;
    }

    auto *tree = new RedBlackIntervalTree();
    if (!roomTable.putNew(id, tree)) {
        delete tree;
        printError("Failed to add room.");
        return;
    }

    printSuccess("Room " + id + " added successfully.");
}

void RoomsManager::removeRoomInteractive() {
    printSectionHeader("Remove Room");

    string id;
    cout << COLOR_PROMPT << "Enter room ID to remove: " << COLOR_RESET;
    cin >> id;

    RedBlackIntervalTree **treePtr = roomTable.get(id);
    if (!treePtr || !(*treePtr)) {
        printError("No room with this ID.");
        return;
    }

    delete *treePtr;
    *treePtr = nullptr;
    roomTable.erase(id);

    printSuccess("Room " + id + " removed.");
}

void RoomsManager::loadRoomBookingsFromFile() const {
    loadBookingsFromFile("data/room_bookings.txt", const_cast<HashMap<string, RedBlackIntervalTree *> &>(roomTable));
}

void RoomsManager::saveRoomBookingsToFile() const {
    saveBookingsToFile("data/room_bookings.txt", roomTable);
}

void RoomsManager::showUserBookings(const std::string &username) const {
    cout << COLOR_MENU << "\nYour room bookings:\n\n" << COLOR_RESET;

    bool any = false;
    const_cast<HashMap<string, RedBlackIntervalTree *> &>(roomTable).forEach(
        [&](const string &roomId, RedBlackIntervalTree * &tree) {
            if (!tree) return;
            tree->forEachInterval([&](const int low, const int high, const string &user) {
                if (user == username) {
                    cout << "  - Room " << roomId
                            << " | Period: [" << low << ", " << high << "]\n";
                    any = true;
                }
            });
        });

    if (!any) printHint("You have no room bookings.");
}

int RoomsManager::collectBookedIntervals(RedBlackIntervalTree *tree, SimpleInterval *arr, int maxCount) {
    int count = 0;
    tree->forEachInterval([&](int low, int high, const string &) {
        if (count < maxCount) {
            arr[count].start = low;
            arr[count].end = high;
            count++;
        }
    });
    return count;
}

void RoomsManager::sortIntervals(SimpleInterval *arr, int n) {
    for (int i = 1; i < n; i++) {
        const SimpleInterval key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].start > key.start) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int RoomsManager::mergeIntervals(SimpleInterval *arr, int n) {
    if (n == 0) return 0;
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (arr[idx].end >= arr[i].start) {
            if (arr[idx].end < arr[i].end) arr[idx].end = arr[i].end;
        } else {
            idx++;
            arr[idx] = arr[i];
        }
    }

    return (idx + 1);
}

void RoomsManager::showRoomsWithAvailableTimes(int openStart, int openEnd) {
    roomTable.forEach([&](const string &roomId, RedBlackIntervalTree * &tree) {
        SimpleInterval intervals[MAX_INTERVALS];

        int count = collectBookedIntervals(tree, intervals, MAX_INTERVALS);
        if (count > 1) sortIntervals(intervals, count);
        count = mergeIntervals(intervals, count);

        int last = openStart;
        cout << COLOR_PROMPT << "Room " << roomId << COLOR_RESET << ": ";
        bool any = false;
        for (int i = 0; i < count; ++i) {
            if (last < intervals[i].start) {
                const int intervalStart = last;
                const int intervalEnd = intervals[i].start - 1;
                if (intervalStart == intervalEnd) {
                    cout << "[" << intervalStart << ", " << (intervalEnd + 1) << "]";
                } else {
                    cout << "[" << intervalStart << ", " << intervalEnd << "] ";
                }
                any = true;
            }
            if (last <= intervals[i].end)
                last = intervals[i].end + 1;
        }
        if (last <= openEnd) {
            if (last == openEnd) {
                cout << "[" << last << "] ";
            } else {
                cout << "[" << last << ", " << openEnd << "] ";
            }
            any = true;
        }
        if (!any) cout << COLOR_ERROR << "(No availability)" << COLOR_RESET;
        cout << "\n";
    });
}
