#include "../../include/managers/RoomsManager.h"
#include "../../include/helpers/UIHelpers.h"
#include <iomanip>
#include <iostream>
#include <sstream>

RoomsManager::RoomsManager() {
  loadRoomsFromFile();
  loadRoomBookingsFromFile();
}

RoomsManager::~RoomsManager() {
  saveRoomsToFile();
  saveRoomBookingsToFile();
  // Data Structure Change
  roomTable.forEach([](const string &id, RedBlackIntervalTree *&tree) {
    delete tree;
    tree = nullptr;
  });
  roomTable.clear();
}

void RoomsManager::loadRoomsFromFile() {
  loadResourceIDsFromFile("data/rooms.txt", roomTable);
}

bool RoomsManager::bookRoom(User *user) {
  printSectionHeader("Book a Study Room");

  // 1. Displaying library rooms
  cout << COLOR_MENU << "Available Study Rooms:\n" << COLOR_RESET;

  bool anyRooms = false;
  roomTable.forEach([&](const string &id, RedBlackIntervalTree *&tree) {
    cout << "  - " << id << "\n";
    anyRooms = true;
  });

  if (!anyRooms) {
    printError("No rooms configured in the system.");
    return false;
  }

  // 2. Room Selection Loop
  string roomchoice;
  RedBlackIntervalTree *tree = nullptr;

  while (true) {
    cout << "\n"
         << COLOR_PROMPT << "Enter Room ID (or '0' to cancel)" << COLOR_RESET
         << ": ";
    cin >> roomchoice;

    if (roomchoice == "0") {
      printHint("Booking cancelled.");
      return false;
    }

    RedBlackIntervalTree **treePtr = roomTable.get(roomchoice);
    if (treePtr && *treePtr) {
      tree = *treePtr;
      break;
    } else {
      printError("Invalid Room ID. Please try again.");
    }
  }

  // 3. Find start and end of interval to call the listAvailableIntervals
  // function
  time_t refStamp = getStartOfYearTimestamp();

  // a) System time - rounded up - Beginning of interval
  tm t = {};
  auto now = std::chrono::system_clock::now();
  time_t now_c = std::chrono::system_clock::to_time_t(now);
  t = *localtime(&now_c);

  if (t.tm_min > 0 || t.tm_sec > 0) {
    t.tm_hour += 1;
    t.tm_min = 0;
    t.tm_sec = 0;
  }
  time_t currentStamp = mktime(&t);
  long long nowSec = static_cast<long long>(difftime(currentStamp, refStamp));

  // b) End of today
  tm endToday = t;
  endToday.tm_hour = 23;
  endToday.tm_min = 59;
  endToday.tm_sec = 59;

  time_t endTodayStamp = mktime(&endToday);
  long long endTodaySec =
      static_cast<long long>(difftime(endTodayStamp, refStamp));

  // c) Start of next day
  tm startTomorrow = t;
  startTomorrow.tm_mday += 1;
  startTomorrow.tm_hour = 0;
  startTomorrow.tm_min = 0;
  startTomorrow.tm_sec = 0;

  time_t startTomorrowStamp = mktime(&startTomorrow);
  long long startTomorrowSec =
      static_cast<long long>(difftime(startTomorrowStamp, refStamp));

  // d) End of next day
  tm endTomorrow = startTomorrow;
  endTomorrow.tm_hour = 23;
  endTomorrow.tm_min = 59;
  endTomorrow.tm_sec = 59;

  time_t endTomorrowStamp = mktime(&endTomorrow);
  long long endTomorrowSec =
      static_cast<long long>(difftime(endTomorrowStamp, refStamp));

  // 4. Display the free intervals for the chosen room
  cout << "\n"
       << COLOR_TITLE << "--- Availability for Room " << roomchoice << " ---"
       << COLOR_RESET << "\n";
  cout << COLOR_DIM << "(Showing free slots for today and tomorrow)"
       << COLOR_RESET << "\n";

  cout << "\n" << COLOR_MENU << "Today:" << COLOR_RESET << "\n";
  tree->listAvailableIntervals(nowSec, endTodaySec);

  cout << "\n" << COLOR_MENU << "Tomorrow:" << COLOR_RESET << "\n";
  tree->listAvailableIntervals(startTomorrowSec, endTomorrowSec);

  // 5. Allow user to enter interval
  printHint("\nEnter booking details below.");

  int sDay, sMonth, sYear, sHour, sMinute;
  int eDay, eMonth, eYear, eHour, eMinute;

  cout << "\nEnter start date and time: \n\n";
  long long startperiod =
      getUserDateAsSeconds(sDay, sMonth, sYear, sHour, sMinute);
  cout << "\nEnter end date and time: \n\n";
  long long endperiod =
      getUserDateAsSeconds(eDay, eMonth, eYear, eHour, eMinute, "end");

  if (startperiod >= endperiod) {
    printError("Invalid interval: End time must be strictly after start time.");
    return false;
  }

  /*
   * Check 1: Today/Tomorrow Only
   * We use the previously calculated 'nowSec' and 'endTomorrowSec'
   * Note: 'nowSec' is the beginning of the next hour from 'now' in the existing
   * logic locally, but strictly speaking 'today' includes now. For strict
   * validation, we should probably allow booking from actual 'now' onwards, but
   * using 'nowSec' (which is start of next hour usually) is safer for
   * alignment, or we can relax it. However, the constraint is "today or
   * tomorrow only". So start must be >= start of today and <= end of tomorrow.
   * Recomputing start of today to be safe, or just use nowSec/endTomorrowSec as
   * boundaries. To avoid confusion with the "rounded up" nowSec, let's use the
   * actual current time offset.
   */

  time_t actualNow = time(nullptr);
  double diffNow = difftime(actualNow, refStamp);
  long long actualNowSec = static_cast<long long>(diffNow);

  // Check 1: Past Booking Forbidden
  if (startperiod < actualNowSec) {
    printError("Cannot book in the past.");
    return false;
  }

  // Check 2: Today/Tomorrow Only
  if (startperiod > endTomorrowSec || endperiod > endTomorrowSec) {
    printError("You can only book for today or tomorrow.");
    return false;
  }

  // Check 3: Max 3 Hours
  long long duration = endperiod - startperiod;
  if (duration > 3 * 3600) {
    printError("Maximum booking duration is 3 hours.");
    return false;
  }

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

  stringstream ss;
  ss << "Room " << roomchoice << " booked successfully from: " << setw(2)
     << setfill('0') << sDay << "/" << setw(2) << setfill('0') << sMonth << "/"
     << sYear << " " << setw(2) << setfill('0') << sHour << ":" << setw(2)
     << setfill('0') << sMinute << " to " << setw(2) << setfill('0') << eDay
     << "/" << setw(2) << setfill('0') << eMonth << "/" << eYear << " "
     << setw(2) << setfill('0') << eHour << ":" << setw(2) << setfill('0')
     << eMinute;

  printSuccess(ss.str());
  printHint("Go to the library help desk at the start of booking period with "
            "your ID.");
  return true;
}

// Non-interactive version for Python API
bool RoomsManager::bookRoomDirect(User *user, const string &roomId,
                                  int startTime, int endTime) {
  if (!user)
    return false;

  RedBlackIntervalTree **treePtr = roomTable.get(roomId);
  if (!treePtr || !(*treePtr)) {
    return false; // Room doesn't exist
  }

  RedBlackIntervalTree *tree = *treePtr;

  // Validation Checks matching interactive mode
  time_t refStamp = getStartOfYearTimestamp();
  time_t actualNow = time(nullptr);
  double diffNow = difftime(actualNow, refStamp);
  long long nowSec = static_cast<long long>(diffNow);

  // Recompute end of tomorrow
  tm t = *localtime(&actualNow);
  t.tm_mday += 1;
  t.tm_hour = 23;
  t.tm_min = 59;
  t.tm_sec = 59;
  time_t endTomorrow = mktime(&t);
  long long endTomorrowSec =
      static_cast<long long>(difftime(endTomorrow, refStamp));

  if (startTime < nowSec)
    return false;
  if (startTime > endTomorrowSec || endTime > endTomorrowSec)
    return false;

  if (startTime >= endTime)
    return false;

  if ((endTime - startTime) > 3 * 3600)
    return false;

  // Check if user already has a conflicting booking
  if (!user->canBookRoom(startTime, endTime)) {
    return false; // User conflict
  }

  // Check if room is available
  if (tree->searchOverlap(startTime, endTime, false)) {
    return false; // Room conflict
  }

  // Book the room
  tree->insert(startTime, endTime, user->getUsername());
  user->addRoomBooking(startTime, endTime);

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

// Non-interactive version for Python API
bool RoomsManager::addRoomDirect(const string &roomId) {
  if (roomTable.contains(roomId)) {
    return false; // Room already exists
  }

  auto *tree = new RedBlackIntervalTree();
  if (!roomTable.putNew(roomId, tree)) {
    delete tree;
    return false;
  }

  return true;
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

// Non-interactive version for Python API
bool RoomsManager::removeRoomDirect(const string &roomId) {
  RedBlackIntervalTree **treePtr = roomTable.get(roomId);
  if (!treePtr || !(*treePtr)) {
    return false; // Room doesn't exist
  }

  delete *treePtr;
  *treePtr = nullptr;
  roomTable.erase(roomId);

  return true;
}

void RoomsManager::loadRoomBookingsFromFile() const {
  loadBookingsFromFile(
      "data/room_bookings.txt",
      const_cast<HashMap<string, RedBlackIntervalTree *> &>(roomTable));
}

void RoomsManager::saveRoomBookingsToFile() const {
  saveBookingsToFile("data/room_bookings.txt", roomTable);
}

void RoomsManager::showUserBookings(const std::string &username) const {
  cout << COLOR_MENU << "\nYour room bookings:\n\n" << COLOR_RESET;

  bool any = false;
  const_cast<HashMap<string, RedBlackIntervalTree *> &>(roomTable).forEach(
      [&](const string &roomId, RedBlackIntervalTree *&tree) {
        if (!tree)
          return;
        tree->forEachInterval(
            [&](const int low, const int high, const string &user) {
              if (user == username) {
                cout << "  - Room " << roomId
                     << " | Period: " << formatTimestamp(low) << " to "
                     << formatTimestamp(high) << "\n";
                any = true;
              }
            });
      });

  if (!any)
    printHint("You have no room bookings.");
}

int RoomsManager::collectBookedIntervals(RedBlackIntervalTree *tree,
                                         SimpleInterval *arr, int maxCount) {
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
  if (n == 0)
    return 0;
  int idx = 0;
  for (int i = 1; i < n; i++) {
    if (arr[idx].end >= arr[i].start) {
      if (arr[idx].end < arr[i].end)
        arr[idx].end = arr[i].end;
    } else {
      idx++;
      arr[idx] = arr[i];
    }
  }

  return (idx + 1);
}

void RoomsManager::showRoomsWithAvailableTimes(int openStart, int openEnd) {
  roomTable.forEach([&](const string &roomId, RedBlackIntervalTree *&tree) {
    SimpleInterval intervals[MAX_INTERVALS];

    int count = collectBookedIntervals(tree, intervals, MAX_INTERVALS);
    if (count > 1)
      sortIntervals(intervals, count);
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
    if (!any)
      cout << COLOR_ERROR << "(No availability)" << COLOR_RESET;
    cout << "\n";
  });
}

void RoomsManager::syncUserBookings(UsersManager &usersManager) {
  roomTable.forEach([&](const string &roomId, RedBlackIntervalTree *&tree) {
    if (!tree)
      return;
    tree->forEachInterval(
        [&](const int low, const int high, const string &username) {
          User *u = usersManager.getUser(username);
          if (u)
            u->addRoomBooking(low, high);
        });
  });
}
