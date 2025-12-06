//
// Created by ZBook 15 Power G7 on 06-Dec-25.
//
#include "managers/BooksManager.h"
#include "managers/LibrarySystem.h"
#include "managers/RoomsManager.h"
#include "managers/UsersManager.h"
#include "models/book.h"
#include "models/user.h"
#include "structures/IntervalTreeComplete.h"
#include "structures/hash_map.h"
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Add and get book") {
  BooksManager manager;

  REQUIRE(manager.addBookDirect("B001", "Title1", "Author1") == true);
  REQUIRE(manager.getBook("B001") != nullptr);

  // Try to add same book again
  REQUIRE(manager.addBookDirect("B001", "Title1", "Author1") == false);
}

TEST_CASE("Remove book") {
  BooksManager manager;
  manager.addBookDirect("B002", "Title2", "Author2");

  REQUIRE(manager.removeBookDirect("B002") == true);
  REQUIRE(manager.getBook("B002") == nullptr);

  // Removing non-existing book
  REQUIRE(manager.removeBookDirect("B999") == false);
}

TEST_CASE("Borrow book non-interactive") {
  BooksManager manager;
  User user("testuser", "password");

  manager.addBookDirect("B003", "Title3", "Author3");

  // First borrow attempt
  REQUIRE(manager.borrowBookDirect(&user, "B003", 1, 2) == true);

  // Overlapping borrow attempt
  REQUIRE(manager.borrowBookDirect(&user, "B003", 1, 2) == false);

  // Borrow non-existing book
  REQUIRE(manager.borrowBookDirect(&user, "B999", 1, 2) == false);

  // Borrow when user cannot book (simulate by filling up bookings)
  manager.addBookDirect("B004", "Title4", "Author4");
  manager.addBookDirect("B005", "Title5", "Author5");

  // User already has B003 from 1 to 2
  REQUIRE(manager.borrowBookDirect(&user, "B004", 1, 2) == true);
  REQUIRE(manager.borrowBookDirect(&user, "B005", 1, 2) == true);

  // Now user has 3 books, should fail next borrow
  manager.addBookDirect("B006", "Title6", "Author6");
  REQUIRE(manager.borrowBookDirect(&user, "B006", 1, 2) == false);
}
