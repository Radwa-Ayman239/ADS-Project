#include "managers/LaptopsManager.h"
#include "models/user.h"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("LaptopsManager add and remove laptop") {
    LaptopsManager laptopsManager;

    // Add a laptop directly
    REQUIRE(laptopsManager.addLaptopDirect("L001") == true);

    // Adding the same laptop again should fail
    REQUIRE(laptopsManager.addLaptopDirect("L001") == false);

    // Remove the laptop
    REQUIRE(laptopsManager.removeLaptopDirect("L001") == true);

    // Removing a non-existent laptop should fail
    REQUIRE(laptopsManager.removeLaptopDirect("L001") == false);
}

TEST_CASE("LaptopsManager borrowLaptopDirect") {
    LaptopsManager laptopsManager;

    // Create mock user
    User mockUser("mockuser", "password");

    // Add a laptop
    REQUIRE(laptopsManager.addLaptopDirect("L002") == true);

    // Borrow the laptop for a time interval
    int start = 1000;
    int end = 2000;
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L002", start, end) == true);

    // Trying to borrow the same laptop for the same interval should fail
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L002", start, end) == false);

    // Borrow for a non-overlapping interval should succeed
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L002", 2001, 3000) == true);

    // Borrow a non-existent laptop should fail
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L999", 1000, 2000) == false);
}

TEST_CASE("LaptopsManager borrowAnyLaptopDirect") {
    LaptopsManager laptopsManager;

    // Create mock users
    User user1("user1", "password");
    User user2("user2", "password");
    User user3("user3", "password");

    // Add multiple laptops
    REQUIRE(laptopsManager.addLaptopDirect("L003") == true);
    REQUIRE(laptopsManager.addLaptopDirect("L004") == true);

    int start = 5000;
    int end = 6000;

    // User1 borrows any available laptop
    std::string bookedId1 = laptopsManager.borrowAnyLaptopDirect(&user1, start, end);
    REQUIRE(!bookedId1.empty());

    // User2 borrows any available laptop for the same interval
    std::string bookedId2 = laptopsManager.borrowAnyLaptopDirect(&user2, start, end);
    REQUIRE(!bookedId2.empty());
    REQUIRE(bookedId2 != bookedId1);

    // No laptops left for the same interval
    std::string bookedId3 = laptopsManager.borrowAnyLaptopDirect(&user3, start, end);
    REQUIRE(bookedId3.empty());
}

TEST_CASE("LaptopsManager overlapping bookings for same user") {
    LaptopsManager laptopsManager;

    // Create mock user
    User mockUser("mockuser2", "password");

    // Add laptops
    REQUIRE(laptopsManager.addLaptopDirect("L005") == true);
    REQUIRE(laptopsManager.addLaptopDirect("L006") == true);

    // Borrow first laptop
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L005", 1000, 2000) == true);

    // Borrow second laptop with overlapping interval should fail due to user conflict
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L006", 1500, 2500) == false);

    // Borrow second laptop with non-overlapping interval should succeed
    REQUIRE(laptopsManager.borrowLaptopDirect(&mockUser, "L006", 2001, 3000) == true);
}
