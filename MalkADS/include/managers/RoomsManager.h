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
    void saveRoomsToFile() const;

    void loadRoomBookingsFromFile() const;

    void saveRoomBookingsToFile() const;

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

    // admin operations
    void addRoomInteractive();

    void removeRoomInteractive();

    void showUserBookings(const std::string &username) const;

    void showRoomsWithAvailableTimes(int openStart, int openEnd);

    void syncUserBookings(UsersManager &usersManager);
};

#endif
