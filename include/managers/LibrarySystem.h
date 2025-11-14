//
// Created by hamdy on 11/10/2025.
//

#ifndef ADS_PROJECT_LIBRARYSYSTEM_H
#define ADS_PROJECT_LIBRARYSYSTEM_H
#include "UserManager.h"
#include "models/User.h"
using namespace std;


class LibrarySystem {
private:
    UserManager user_manager_;
    User *currentUser;

    static void showMenu() ;

    void handleChoice(const string &choice);

public:
    LibrarySystem();

    void run();
};


#endif //ADS_PROJECT_LIBRARYSYSTEM_H
