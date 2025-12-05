//
// Created by hamdy on 11/20/2025.
//

#ifndef MALKADS_RESOURCEIO_H
#define MALKADS_RESOURCEIO_H

#include "../structures//IntervalTreeComplete.h"
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

inline void parseDate(string stringDate, int &day, int &month, int &year) {
  int first = stringDate.find("/");
  int second = stringDate.find("/", first + 1);

  string dayStr = stringDate.substr(0, first);
  string monthStr = stringDate.substr(first + 1, second - first - 1);
  string yearStr = stringDate.substr(second + 1);

  day = stoi(dayStr);
  month = stoi(monthStr);
  year = stoi(yearStr);
}

inline void parseTime(string stringTime, int &hour, int &minute) {
  int colon = stringTime.find(":");

  string hourStr = stringTime.substr(0, colon);
  string minuteStr = stringTime.substr(colon + 1);

  hour = stoi(hourStr);
  minute = stoi(minuteStr);
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

  std::stringstream ss;
  ss << std::put_time(&targetTm, "%d/%m/%Y %H:%M");
  return ss.str();
}

inline long long getUserDateAsSeconds(int &day, int &month, int &year,
                                      int &hour, int &minute) {
  string startDateStr, startTimeStr;
  cout << COLOR_PROMPT << "\tEnter start date (dd/mm/yyyy): " << COLOR_RESET;
  cin >> startDateStr;
  cout << COLOR_PROMPT << "\tEnter start time (hh:mm): " << COLOR_RESET;
  cin >> startTimeStr;

  parseDate(startDateStr, day, month, year);
  parseTime(startTimeStr, hour, minute);

  tm userTime = {};
  userTime.tm_year = year - 1900;
  userTime.tm_mon = month - 1;
  userTime.tm_mday = day;
  userTime.tm_hour = hour;
  userTime.tm_min = minute;
  userTime.tm_sec = 0;

  time_t userStamp = mktime(&userTime);

  time_t refStamp = getStartOfYearTimestamp();
  double diff = difftime(userStamp, refStamp);

  return static_cast<long long>(diff);
}

#endif // MALKADS_RESOURCEIO_H