//
// Created by hamdy on 11/20/2025.
//

#ifndef MALKADS_RESOURCEIO_H
#define MALKADS_RESOURCEIO_H

#include "../structures/IntervalTreeComplete.h"
#include "../structures/hash_map.h"
#include "UIHelpers.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//
// Generic helpers for resources that are stored as:
//   HashMap<string, RedBlackIntervalTree*>
// and booking files that use lines like:
//   id,start,end,username
//

// Load plain IDs (rooms.txt, laptops.txt, etc.)

template <typename MapType>
void loadResourceIDsFromFile(const string &path, MapType &table) {
  ifstream file(path);
  if (!file) {
    cout << "Error opeing " << path << "\n";
    return;
  }

  string id;
  while (getline(file, id)) {
    if (id.empty())
      continue;
    auto *tree = new RedBlackIntervalTree();
    table.putNew(id, tree);
  }
}

template <typename MapType>
void saveResourceIDsToFile(const string &path, const MapType &table) {
  ofstream file(path, ios::out | ios::trunc);
  if (!file) {
    std::cout << "Error opening " << path << " for writing\n";
    return;
  }

  const_cast<MapType &>(table).forEach(
      [&](const string &id, RedBlackIntervalTree *&tree) {
        file << id << "\n";
      });
}

template <typename MapType>
void loadBookingsFromFile(const string &path, MapType &table) {
  std::ifstream file(path);
  if (!file)
    return;

  string line;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    size_t c1 = line.find(',');
    if (c1 == string::npos)
      continue;
    size_t c2 = line.find(',', c1 + 1);
    if (c2 == string::npos)
      continue;
    size_t c3 = line.find(',', c2 + 1);
    if (c3 == string::npos)
      continue;

    string id = line.substr(0, c1);
    string startStr = line.substr(c1 + 1, c2 - (c1 + 1));
    string endStr = line.substr(c2 + 1, c3 - (c2 + 1));
    string user = line.substr(c3 + 1);

    int start = std::stoi(startStr);
    int end = std::stoi(endStr);

    RedBlackIntervalTree **treePtr = table.get(id);
    if (!treePtr || !(*treePtr))
      continue;

    RedBlackIntervalTree *tree = *treePtr;
    tree->insert(start, end, user);
  }
}

template <typename MapType>
void saveBookingsToFile(const string &path, const MapType &table) {
  ofstream file(path, ios::out | ios::trunc);
  if (!file) {
    std::cout << "Error opening " << path << " for writing\n";
    return;
  }

  const_cast<MapType &>(table).forEach(
      [&](const string &id, RedBlackIntervalTree *&tree) {
        if (!tree)
          return;
        tree->forEachInterval([&](const int low, const int high,
                                  const string &username) {
          file << id << "," << low << "," << high << "," << username << "\n";
        });
      });
}

inline bool isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

inline bool isValidDate(int d, int m, int y) {
  if (y < 2025 || y > 2100)
    return false;
  if (m < 1 || m > 12)
    return false;
  if (d < 1)
    return false;

  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (isLeapYear(y))
    daysInMonth[2] = 29;

  return d <= daysInMonth[m];
}

inline bool isValidTime(int h, int m) {
  return (h >= 0 && h <= 23) && (m >= 0 && m <= 59);
}

inline bool parseDate(const string &stringDate, int &day, int &month,
                      int &year) {
  if (stringDate.empty())
    return false;
  size_t first = stringDate.find("/");
  if (first == string::npos)
    return false;
  size_t second = stringDate.find("/", first + 1);
  if (second == string::npos)
    return false;

  try {
    string dayStr = stringDate.substr(0, first);
    string monthStr = stringDate.substr(first + 1, second - first - 1);
    string yearStr = stringDate.substr(second + 1);

    // Check if numeric
    if (dayStr.find_first_not_of("0123456789") != string::npos)
      return false;
    if (monthStr.find_first_not_of("0123456789") != string::npos)
      return false;
    if (yearStr.find_first_not_of("0123456789") != string::npos)
      return false;

    day = stoi(dayStr);
    month = stoi(monthStr);
    year = stoi(yearStr);

    return isValidDate(day, month, year);
  } catch (...) {
    return false;
  }
}

inline bool parseTime(const string &stringTime, int &hour, int &minute) {
  if (stringTime.empty())
    return false;
  size_t colon = stringTime.find(":");
  if (colon == string::npos)
    return false;

  try {
    string hourStr = stringTime.substr(0, colon);
    string minuteStr = stringTime.substr(colon + 1);

    if (hourStr.find_first_not_of("0123456789") != string::npos)
      return false;
    if (minuteStr.find_first_not_of("0123456789") != string::npos)
      return false;

    hour = stoi(hourStr);
    minute = stoi(minuteStr);

    return isValidTime(hour, minute);
  } catch (...) {
    return false;
  }
}

inline time_t getStartOfYearTimestamp() {
  tm ref = {};
  ref.tm_year = 2025 - 1900;
  ref.tm_mon = 0;
  ref.tm_mday = 1;
  ref.tm_hour = 0;
  ref.tm_min = 0;
  ref.tm_sec = 0;

  return mktime(&ref);
}

inline string formatTimestamp(long long offsetSeconds) {
  time_t refStamp = getStartOfYearTimestamp();
  time_t targetStamp = refStamp + offsetSeconds;

  tm targetTm = *std::localtime(&targetStamp);

  stringstream ss;
  ss << put_time(&targetTm, "%d/%m/%Y %H:%M");
  return ss.str();
}

inline long long getUserDateAsSeconds(int &day, int &month, int &year,
                                      int &hour, int &minute,
                                      const string &label = "start") {
  while (true) {
    string startDateStr, startTimeStr;
    cout << COLOR_PROMPT << "\tEnter " << label
         << " date (dd/mm/yyyy): " << COLOR_RESET;
    cin >> startDateStr;

    // Clear failure state/buffer if bad input led to this
    if (cin.fail()) {
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }

    cout << COLOR_PROMPT << "\tEnter " << label
         << " time (hh:mm): " << COLOR_RESET;
    cin >> startTimeStr;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }

    if (!parseDate(startDateStr, day, month, year)) {
      printError("Invalid date format or value. Please use dd/mm/yyyy (e.g. "
                 "15/05/2025).");
      continue;
    }

    if (!parseTime(startTimeStr, hour, minute)) {
      printError(
          "Invalid time format or value. Please use hh:mm (00:00 to 23:59).");
      continue;
    }

    tm userTime = {};
    userTime.tm_year = year - 1900;
    userTime.tm_mon = month - 1;
    userTime.tm_mday = day;
    userTime.tm_hour = hour;
    userTime.tm_min = minute;
    userTime.tm_sec = 0;
    userTime.tm_isdst = -1; // Let system determine DST

    time_t userStamp = mktime(&userTime);
    if (userStamp == -1) {
      printError("Could not convert date/time.");
      continue;
    }

    // Check against current time (past check)
    time_t now = time(nullptr);
    if (difftime(userStamp, now) < 0) {
      printError("You cannot select a date/time in the past.");
      continue;
    }

    time_t refStamp = getStartOfYearTimestamp();
    double diff = difftime(userStamp, refStamp);

    if (diff < 0) {
      printError(
          "Date cannot be before the start of the simulation year (2025).");
      continue;
    }

    return static_cast<long long>(diff);
  }
}

#endif // MALKADS_RESOURCEIO_H