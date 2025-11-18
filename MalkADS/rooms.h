#ifndef ROOMS_H
#define ROOMS_H

#include <string>
#include "IntervalTreeComplete.h"
#include <unordered_map>
using namespace std;

class Rooms {
private:
//Data Structure Change
    unordered_map<string, RedBlackIntervalTree*> roomTable;

public:
    Rooms(); 
    ~Rooms(); 

   
    void loadRoomsFromFile(); // read room IDs from rooms.txt and create interval trees
    bool bookRoom();


};

#endif
