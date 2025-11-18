#ifndef ROOMS_H
#define ROOMS_H

#include <string>
#include "../structures/IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class RoomsManager {
private:
//Data Structure Change
    unordered_map<string, RedBlackIntervalTree*> roomTable;

public:
    RoomsManager();
    ~RoomsManager();

   
    void loadRoomsFromFile(); // read room IDs from rooms.txt and create interval trees
    bool bookRoom();


};

#endif
