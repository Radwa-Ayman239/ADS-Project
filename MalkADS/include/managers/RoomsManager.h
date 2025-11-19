#ifndef ROOMS_H
#define ROOMS_H

#include <string>
#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
using namespace std;

class RoomsManager {
private:
    //Data Structure Change
    HashMap<string, RedBlackIntervalTree *> roomTable;

    void loadRoomsFromFile(); // read room IDs from rooms.txt and create interval trees
    void saveRoomsToFile() const;

    void loadRoomBookingsFromFile() const;
    void saveRoomBookingsToFile();
public:
    RoomsManager();

    ~RoomsManager();

    bool bookRoom();

    // admin operations
    void addRoomInteractive();

    void removeRoomInteractive();
};

#endif
