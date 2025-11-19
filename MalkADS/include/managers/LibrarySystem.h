//
// Created by hamdy on 11/19/2025.
//

#ifndef ADS_PROJECT_LIBRARYSYSTEM_H
#define ADS_PROJECT_LIBRARYSYSTEM_H
#include "LaptopsManager.h"
#include "RoomsManager.h"
#include "UsersManager.h"
#include "BooksManager.h"


class LibrarySystem {
public:
    LibrarySystem();
    void run();

private:
    UsersManager users;
    RoomsManager rooms;
    LaptopsManager laptops;
    BooksManager books;

    void loadData();

    static void showUserMenu();
    static void showAdminMenu();
};


#endif //ADS_PROJECT_LIBRARYSYSTEM_H