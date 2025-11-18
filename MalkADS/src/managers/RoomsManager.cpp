#include "../../include/managers/RoomsManager.h"
#include <fstream>
#include <iostream>

RoomsManager::RoomsManager() {
}

RoomsManager::~RoomsManager() {
    //Data Structure Change
    unordered_map<string, RedBlackIntervalTree*>::iterator it;

    for (it = roomTable.begin(); it != roomTable.end(); ++it) {
        delete it->second; 
    }

    roomTable.clear();
}



void RoomsManager::loadRoomsFromFile() {
    ifstream file("data\\rooms.txt");
    if (!file) {
        cout << "Error opening rooms.txt\n";
        return;
    }

    string roomID;
    while (getline(file, roomID)) {
        RedBlackIntervalTree* tree = new RedBlackIntervalTree();
        //Data Structure Change
        roomTable[roomID] = tree;
    }

    file.close();
}


bool RoomsManager::bookRoom() {
    //1.Displaying libaray rooms and having user pick which room they want to book:

    //Data Structure Change
    unordered_map<string, RedBlackIntervalTree*>::iterator it;
    string roomchoice;

    cout <<"Library Study Rooms: \n";
   
    for (it = roomTable.begin(); it != roomTable.end(); ++it) {
        cout << "- " << it->first << endl;
    }
    cout << "\nEnter Room ID: "; cin >> roomchoice; cout << "\n";


    //2.Dislay the free intevals for the chosen room
       //Data Structure Change
    it = roomTable.find(roomchoice);
    if (it == roomTable.end()) {
        cout << "No such room.\n";
    } else {
        cout << "\nFree periods: \n";
        it->second->listAvailableIntervals(0, 20); //YOU MUST CHANGE THESEE!!
        //START AND END DATE MUST BE CALCULATED FROM CURRENT MOMENT!! (this will be done later just use random numbers for now )
    }

    //3.Allow use to enter interval
    int startperiod; int endperiod;
    cout << "\n\nNow enter your desired booking period";
    cout << "\nStart of booking period: "; cin >> startperiod;
    cout << "End of booking period: "; cin >> endperiod;
    //NEED TO CHANGE THIS TO ENTERING ACTUAL TIMEEEEE

    //4.If no conflict - add to tree
        //Data Structure Change
    if (roomTable[roomchoice]->searchOverlap(startperiod, endperiod, true) == true) {
        return false;
    }
    else {
        roomTable[roomchoice]->insert(startperiod, endperiod);
        return true;
    }
}



