#include "models/user.h"
#include "managers/UsersManager.h"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

// Extend UsersManager to access protected userTable
class UsersManagerTestable : public UsersManager {
public:
    void addUserMock(const User& u) {
        userTable.putNew(u.getUsername(), u);
    }
};

TEST_CASE("UsersManager login and mock users") {
    UsersManagerTestable usersManager;

    // Create mock users
    User user1("alice", "pass123");
    User user2("bob", "mypassword");
    User user3("charlie", "secret");

    // Add mock users
    usersManager.addUserMock(user1);
    usersManager.addUserMock(user2);
    usersManager.addUserMock(user3);

    // Login with correct credentials
    User* u1 = usersManager.login("alice", "pass123");
    REQUIRE(u1 != nullptr);
    REQUIRE(u1->getUsername() == "alice");

    User* u2 = usersManager.login("bob", "mypassword");
    REQUIRE(u2 != nullptr);
    REQUIRE(u2->getUsername() == "bob");

    // Login with wrong password
    User* wrongPass = usersManager.login("charlie", "wrong");
    REQUIRE(wrongPass == nullptr);

    // Login with non-existing username
    User* notExist = usersManager.login("david", "any");
    REQUIRE(notExist == nullptr);
}

TEST_CASE("UsersManager admin and normal user") {
    UsersManagerTestable usersManager;

    User admin("adminuser", "adminpass", true);
    User normal("normaluser", "userpass", false);

    usersManager.addUserMock(admin);
    usersManager.addUserMock(normal);

    User* a = usersManager.login("adminuser", "adminpass");
    REQUIRE(a != nullptr);
    REQUIRE(a->getIsAdmin() == true);

    User* n = usersManager.login("normaluser", "userpass");
    REQUIRE(n != nullptr);
    REQUIRE(n->getIsAdmin() == false);
}
