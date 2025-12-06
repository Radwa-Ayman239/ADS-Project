// Approach1:
// #include "managers/BooksManager.h"
// #include "managers/LibrarySystem.h"
// #include "managers/RoomsManager.h"
// #include "managers/UsersManager.h"
// #include "models/book.h"
// #include "models/user.h"
// #include "structures/IntervalTreeComplete.h"
// #include "structures/hash_map.h"
// #include <catch2/catch_all.hpp>
// #include <catch2/catch_test_macros.hpp>
//
// TEST_CASE("RoomsManager add and book room") {
//     RoomsManager roomsManager;
//     UsersManager usersManager;
//
//     // Load users from file
//     usersManager.loadUsersFromFile();
//
//     // Pick an existing user from users.txt
//     User *user = usersManager.login("testuser", "password"); // make sure
//     this exists in users.txt REQUIRE(user != nullptr);
//
//     // Add a room directly
//     REQUIRE(roomsManager.addRoomDirect("R001") == true);
//
//     // Trying to add the same room again should fail
//     REQUIRE(roomsManager.addRoomDirect("R001") == false);
//
//     // Book the room
//     int start = 1000;
//     int end = 2000;
//     REQUIRE(roomsManager.bookRoomDirect(user, "R001", start, end) == true);
//
//     // Trying to book overlapping interval should fail
//     REQUIRE(roomsManager.bookRoomDirect(user, "R001", start, end) == false);
//
//     // Booking a non-existing room should fail
//     REQUIRE(roomsManager.bookRoomDirect(user, "R999", start, end) == false);
// }
//
// TEST_CASE("RoomsManager remove room") {
//     RoomsManager roomsManager;
//
//     // Add a room first
//     REQUIRE(roomsManager.addRoomDirect("R002") == true);
//
//     // Remove the room
//     REQUIRE(roomsManager.removeRoomDirect("R002") == true);
//
//     // Removing again should fail
//     REQUIRE(roomsManager.removeRoomDirect("R002") == false);
// }

// Approach 2
#include "managers/RoomsManager.h"
#include "managers/UsersManager.h"
#include "models/user.h"
#include "structures/IntervalTreeComplete.h"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <ctime>

// Helper to get valid future timestamps
void getFutureInterval(int &start, int &end, int duration = 3600) {
  time_t now = time(nullptr);
  time_t ref = getStartOfYearTimestamp();
  long long offset = static_cast<long long>(difftime(now, ref));

  // Start 2 hours from now to be safe
  start = static_cast<int>(offset + 7200);
  end = start + duration;
}

TEST_CASE("RoomsManager add and book room with mock user") {
  RoomsManager roomsManager;

  // Create a mock user directly
  User mockUser("mockuser", "password");

  // Add a room directly
  REQUIRE(roomsManager.addRoomDirect("R001") == true);

  // Trying to add the same room again should fail
  REQUIRE(roomsManager.addRoomDirect("R001") == false);

  // Book the room
  int start, end;
  getFutureInterval(start, end);

  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", start, end) == true);

  // Trying to book overlapping interval should fail
  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", start, end) == false);

  // Booking a non-existing room should fail
  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R999", start, end) == false);

  // Booking a different interval should succeed (if no conflict with first)
  // First booking was [start, start+3600]
  // Second booking: [end+60, end+3660]
  int start2 = end + 60;
  int end2 = start2 + 3600;

  // Ensure check user limit (3 rooms? no, 3 books. Rooms might be 1?)
  // Rooms definition: "You already have another room booked during this
  // period." And "canBookRoom(start, end)". If it allows concurrent or
  // non-overlapping bookings? UsersManager defines limits. Assuming mock user
  // has capacity.
  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", start2, end2) == true);
}

TEST_CASE("RoomsManager remove room") {
  RoomsManager roomsManager;

  // Add a room first
  REQUIRE(roomsManager.addRoomDirect("R002") == true);

  // Remove the room
  REQUIRE(roomsManager.removeRoomDirect("R002") == true);

  // Removing again should fail
  REQUIRE(roomsManager.removeRoomDirect("R002") == false);
}

TEST_CASE("Booking conflict for mock user") {
  RoomsManager roomsManager;

  // Create a mock user
  User mockUser("user1", "password");

  // Add a room
  REQUIRE(roomsManager.addRoomDirect("R003") == true);

  // Book one interval
  int start, end;
  getFutureInterval(start, end, 1800); // 30 min duration

  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", start, end) == true);

  // Overlapping interval for same user should fail (conflict or user limit)
  int startOverlap = start + 600; // 10 mins in
  int endOverlap = end + 600;
  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", startOverlap,
                                      endOverlap) == false);

  // Non-overlapping interval should succeed
  int start2 = end + 3600;
  int end2 = start2 + 1800;
  REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", start2, end2) == true);
}
