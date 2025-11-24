#ifndef ROOMS_H
#define ROOMS_H

#include <string>

#include "UsersManager.h"
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
#include "../models/user.h"
#include "../helpers/ResourceIO.h"
using namespace std;

class RoomsManager {
private:
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> roomTable;

    void loadRoomsFromFile(); // read room IDs from rooms.txt and create interval trees
    
    void loadRoomBookingsFromFile() const;

    struct SimpleInterval {
        int start;
        int end;
    };

    static constexpr int MAX_INTERVALS = 64;

    static int collectBookedIntervals(RedBlackIntervalTree *tree, SimpleInterval *arr, int maxCount);

    static void sortIntervals(SimpleInterval *arr, int n);

    static int mergeIntervals(SimpleInterval *arr, int n);

public:
    RoomsManager();

    ~RoomsManager();

    bool bookRoom(User *user);
    
    // Non-interactive version for Python API
    bool bookRoomDirect(User *user, const string& roomId, int startTime, int endTime);

    // admin operations
    void addRoomInteractive();
    
    // Non-interactive version for Python API
    bool addRoomDirect(const string& roomId);

    void removeRoomInteractive();
    
    // Non-interactive version for Python API
    bool removeRoomDirect(const string& roomId);
    
    // Save methods - made public for Python access
    void saveRoomsToFile() const;
    void saveRoomBookingsToFile() const;

    void showUserBookings(const std::string &username) const;

    void showRoomsWithAvailableTimes(int openStart, int openEnd);

    void syncUserBookings(UsersManager &usersManager);
    
    // Iterator for Python bindings - callback receives (roomId, start, end, username)
    template<typename Func>
    void forEachBooking(Func func) {
        roomTable.forEach([&](const string& roomId, RedBlackIntervalTree*& tree) {
            if (!tree) return;
            tree->forEachInterval([&](int low, int high, const string& username) {
                func(roomId, low, high, username);
            });
        });
    }
    
    // Iterator for all rooms
    template<typename Func>
    void forEachRoom(Func func) {
        roomTable.forEach([&](const string& roomId, RedBlackIntervalTree*& tree) {
            func(roomId);
        });
    }
    
    // Get bookings for specific room
    template<typename Func>
    void getRoomBookings(const string& roomId, Func func) {
        RedBlackIntervalTree** treePtr = roomTable.get(roomId);
        if (!treePtr || !(*treePtr)) return;
        
        (*treePtr)->forEachInterval([&](int low, int high, const string& username) {
            func(low, high, username);
        });
    }
};

#endif
