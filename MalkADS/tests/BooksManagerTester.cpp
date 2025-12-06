//
// Created by ZBook 15 Power G7 on 06-Dec-25.
//
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "BooksManager.h"
#include "LibrarySystem.h"
#include "UsersManager.h"
#include "RoomsManager.h"
#include "Book.h"
#include "hashmap.h"
#include "IntervalTreeComplete.h"
#include "User.h"
#include

class MockIntervalTree {
public:
    bool inserted = false;
    bool searchOverlapCalled = false;

    bool searchOverlap(int start, int end, bool) {
        searchOverlapCalled = true;
        return inserted;
    }

    void insert(int start, int end, const std::string&) {
        inserted = true;
    }

    template <typename Func>
    void forEachInterval(Func f) {
        if (inserted) {
            f(1, 2, "testuser");
        }
    }
};

class MockUser : public User {
public:
    bool canBook = true;
    std::vector<std::pair<int,int>> bookings;

    bool canBookBook(int start, int end) override { return canBook; }

    void addBookBooking(int start, int end) override {
        bookings.push_back({start, end});
    }

    std::string getUsername() override { return "testuser"; }
};

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
    MockUser user;

    manager.addBookDirect("B003", "Title3", "Author3");

    // First borrow attempt
    REQUIRE(manager.borrowBookDirect(&user, "B003", 1, 2) == true);

    // Overlapping borrow attempt
    REQUIRE(manager.borrowBookDirect(&user, "B003", 1, 2) == false);

    // Borrow non-existing book
    REQUIRE(manager.borrowBookDirect(&user, "B999", 1, 2) == false);

    // Borrow when user cannot book
    user.canBook = false;
    REQUIRE(manager.borrowBookDirect(&user, "B003", 3, 4) == false);
}

TEST_CASE("Sync user bookings") {
    BooksManager manager;
    MockUser user;

    manager.addBookDirect("B004", "Title4", "Author4");

    // Manually insert a booking into BookTable
    RedBlackIntervalTree **treePtr = manager.BookTable.get("B004");
    MockIntervalTree tree;
    *treePtr = reinterpret_cast<RedBlackIntervalTree*>(&tree);
    tree.insert(5,10,"testuser");

    // Mock UsersManager
    class MockUsersManager {
    public:
        MockUser* getUser(const std::string&) { return &user; }
    } usersManager;

    manager.syncUserBookings(usersManager);
    REQUIRE(user.bookings.size() == 1);
}
