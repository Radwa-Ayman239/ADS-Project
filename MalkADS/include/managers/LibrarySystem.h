#ifndef ADS_PROJECT_LIBRARYSYSTEM_H
#define ADS_PROJECT_LIBRARYSYSTEM_H


#include "LaptopsManager.h"
#include "RoomsManager.h"
#include "UsersManager.h"
#include "BooksManager.h"
#include "../models/user.h"   // make sure this is visible

class LibrarySystem {
public:
    LibrarySystem();

    void run();

private:
    UsersManager users;
    RoomsManager rooms;
    LaptopsManager laptops;
    BooksManager books;

    // track currently logged-in user
    User *currentUser = nullptr;

    void loadData();

    static void showUserMenu();

    static void showAdminMenu();

    // new modular helpers
    bool loginLoop();

    void userSession();

    void handleUserChoice(int choice);

    void handleAdminChoice(int choice);

    bool isCurrentUserAdmin() const {
        return currentUser && currentUser->getIsAdmin();
    };
};

#endif //ADS_PROJECT_LIBRARYSYSTEM_H
