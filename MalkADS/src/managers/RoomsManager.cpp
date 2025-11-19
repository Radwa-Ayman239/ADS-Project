#include "../../include/managers/RoomsManager.h"
#include <fstream>
#include <iostream>

RoomsManager::RoomsManager() {
    loadRoomsFromFile();
}

RoomsManager::~RoomsManager() {
    saveRoomsToFile();
    //Data Structure Change
    roomTable.forEach([](const string &id, RedBlackIntervalTree * &tree) {
        delete tree;
        tree = nullptr;
    });
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
        RedBlackIntervalTree *tree = new RedBlackIntervalTree();
        //Data Structure Change
        roomTable.putNew(roomID, tree);
    }

    file.close();
}


bool RoomsManager::bookRoom() {
    //1.Displaying libaray rooms and having user pick which room they want to book:

    //Data Structure Change
    string roomchoice;

    cout << "Library Study Rooms: \n";

    roomTable.forEach([&](const string &id, RedBlackIntervalTree * &tree) {
        cout << "- " << id << "\n";
    });

    cout << "\nEnter Room ID: ";
    cin >> roomchoice;
    cout << "\n";


    //2.Dislay the free intevals for the chosen room
    //Data Structure Change
    RedBlackIntervalTree **treePtr = roomTable.get(roomchoice);
    if (!treePtr || !(*treePtr)) {
        cout << "No such room.\n";
        return false;
    }

    RedBlackIntervalTree *tree = *treePtr;

    //3.Allow use to enter interval
    int startperiod;
    int endperiod;
    cout << "\n\nNow enter your desired booking period";
    cout << "\nStart of booking period: ";
    cin >> startperiod;
    cout << "End of booking period: ";
    cin >> endperiod;
    //NEED TO CHANGE THIS TO ENTERING ACTUAL TIMEEEEE

    //4.If no conflict - add to tree
    //Data Structure Change
    if (tree->searchOverlap(startperiod, endperiod, true)) {
        return false;
    } else {
        tree->insert(startperiod, endperiod);
        return true;
    }
}

void RoomsManager::saveRoomsToFile() const {
    ofstream file("data/rooms.txt", ios::out | ios::trunc);
    if (!file) {
        cout << "Error opening rooms.txt for writing\n";
        return;
    }

    const_cast<HashMap<string, RedBlackIntervalTree*>&>(roomTable).forEach([&](const string &id, RedBlackIntervalTree* &tree) {
        file << id << "\n";
    });

    file.close();
}

void RoomsManager::addRoomInteractive() {
    string id;
    cout << "\nEnter new room ID: ";
    cin >> id;

    if (roomTable.contains(id)) {
        cout << "A room with this ID already exists.\n";
        return;
    }

    auto* tree = new RedBlackIntervalTree();
    if (!roomTable.putNew(id, tree)) {
        delete tree;
        cout << "Failed to add room.\n";
        return;
    }

    cout << "Room " << id << " added successfully.\n";
}

void RoomsManager::removeRoomInteractive() {
    string id;
    cout << "\nEnter room ID to remove: ";
    cin >> id;

    RedBlackIntervalTree** treePtr = roomTable.get(id);
    if (!treePtr || !(*treePtr)) {
        cout << "No room with this ID.\n";
        return;
    }

    delete *treePtr;
    *treePtr = nullptr;

    roomTable.erase(id);
    cout << "Room " << id << " removed.\n";
}



