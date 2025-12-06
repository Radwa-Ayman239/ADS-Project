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
//     User *user = usersManager.login("testuser", "password"); // make sure this exists in users.txt
//     REQUIRE(user != nullptr);
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

//Approach 2
#include "managers/RoomsManager.h"
#include "managers/UsersManager.h"
#include "models/user.h"
#include "structures/IntervalTreeComplete.h"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("RoomsManager add and book room with mock user") {
    RoomsManager roomsManager;

    // Create a mock user directly
    User mockUser("mockuser", "password");

    // Add a room directly
    REQUIRE(roomsManager.addRoomDirect("R001") == true);

    // Trying to add the same room again should fail
    REQUIRE(roomsManager.addRoomDirect("R001") == false);

    // Book the room
    int start = 1000;
    int end = 2000;
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", start, end) == true);

    // Trying to book overlapping interval should fail
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", start, end) == false);

    // Booking a non-existing room should fail
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R999", start, end) == false);

    // Booking a different interval should succeed
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R001", 2001, 3000) == true);
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
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", 5, 10) == true);

    // Overlapping interval for same user should fail
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", 8, 12) == false);

    // Non-overlapping interval should succeed
    REQUIRE(roomsManager.bookRoomDirect(&mockUser, "R003", 11, 15) == true);
}
