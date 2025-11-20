//
// Created by hamdy on 11/19/2025.
//

#include "../../include/managers/LibrarySystem.h"
#include "../../include/helpers/UIHelpers.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

LibrarySystem::LibrarySystem() {
    loadData();
}

void LibrarySystem::loadData() {
    users.loadUsersFromFile();
}

void LibrarySystem::showUserMenu() {
    clearScreen();
    printLine();
    printCentered("AUC Library System - User Menu");
    printLine();

    cout << COLOR_MENU << "\n  Available actions:\n\n" << COLOR_RESET;

    cout << "  " << COLOR_PROMPT << "1" << COLOR_RESET << "  Book a room\n";
    cout << "  " << COLOR_PROMPT << "2" << COLOR_RESET << "  Borrow a laptop\n";
    cout << "  " << COLOR_PROMPT << "3" << COLOR_RESET << "  Borrow a book\n";
    cout << "  " << COLOR_PROMPT << "4" << COLOR_RESET << "  View my bookings\n";
    cout << "  " << COLOR_PROMPT << "5" << COLOR_RESET << "  Log out\n\n";

    printLine('-');
    cout << COLOR_PROMPT << "Enter your choice" << COLOR_RESET << ": ";
}

void LibrarySystem::showAdminMenu() {
    clearScreen();
    printLine();
    printCentered("AUC Library System - Admin Menu");
    printLine();

    cout << COLOR_MENU << "\n  Management actions:\n\n" << COLOR_RESET;

    cout << "  " << COLOR_PROMPT << "1" << COLOR_RESET << "  Add a new book\n";
    cout << "  " << COLOR_PROMPT << "2" << COLOR_RESET << "  Remove a book\n";
    cout << "  " << COLOR_PROMPT << "3" << COLOR_RESET << "  Add a laptop\n";
    cout << "  " << COLOR_PROMPT << "4" << COLOR_RESET << "  Remove a laptop\n";
    cout << "  " << COLOR_PROMPT << "5" << COLOR_RESET << "  Add a room\n";
    cout << "  " << COLOR_PROMPT << "6" << COLOR_RESET << "  Remove a room\n";
    cout << "  " << COLOR_PROMPT << "7" << COLOR_RESET << "  Book a room (admin)\n";
    cout << "  " << COLOR_PROMPT << "8" << COLOR_RESET << "  Borrow a laptop (admin)\n";
    cout << "  " << COLOR_PROMPT << "9" << COLOR_RESET << "  Borrow a book (admin)\n";
    cout << "  " << COLOR_PROMPT << "10" << COLOR_RESET << " Log out\n\n";

    printLine('-');
    cout << COLOR_PROMPT << "Enter your choice" << COLOR_RESET << ": ";
}

bool LibrarySystem::loginLoop() {
    string username;
    string password;

    clearScreen();
    printLine();
    printCentered("Welcome to AUC Library System");
    printLine();

    cout << COLOR_DIM << "\n(Type 'quit' as username to exit the program)\n\n" << COLOR_RESET;

    cout << COLOR_PROMPT << "Username" << COLOR_RESET << ": ";
    cin >> username;

    if (username == "quit") {
        cout << "\n" << COLOR_DIM << "Exiting program..." << COLOR_RESET << "\n";
        return false;
    }

    cout << COLOR_PROMPT << "Password" << COLOR_RESET << ": ";
    cin >> password;

    User *loggedIn = users.login(username, password);
    if (!loggedIn) {
        cout << "\n" << COLOR_ERROR << "Incorrect username or password. Try again."
                << COLOR_RESET << "\n";
        currentUser = nullptr;
        // short pause effect
        cout << COLOR_DIM << "(Press Enter to continue...)" << COLOR_RESET;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin.get();
        return true;
    }

    currentUser = loggedIn;
    cout << "\n" << COLOR_TITLE << "Login successful! "
            << "(" << (isCurrentUserAdmin() ? "admin" : "user") << ")"
            << COLOR_RESET << "\n";

    // brief pause so user can see the message
    cout << COLOR_DIM << "(Press Enter to continue...)" << COLOR_RESET;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();

    return true;
}

void LibrarySystem::userSession() {
    bool inSession = true;

    while (inSession) {
        int choice;

        if (isCurrentUserAdmin()) {
            showAdminMenu();
        } else {
            showUserMenu();
        }

        cin >> choice;
        cout << "\n";

        if (isCurrentUserAdmin()) {
            handleAdminChoice(choice);
            if (choice == 10) inSession = false;
        } else {
            handleUserChoice(choice);
            if (choice == 5) inSession = false;
        }

        if (inSession) {
            char again;
            cout << "\n\n" << COLOR_PROMPT
                    << "Do you want to perform another action? (y or n): "
                    << COLOR_RESET;
            cin >> again;
            if (again == 'n' || again == 'N') {
                cout << "\n" << COLOR_DIM << "Logging out..." << COLOR_RESET << "\n";
                inSession = false;
            }
        }
    }

    currentUser = nullptr;
}

void LibrarySystem::handleUserChoice(const int choice) {
    if (choice == 1) {
        cout << "\n================================= Booking a Room =================================\n";
        const bool ok = rooms.bookRoom(currentUser);
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 2) {
        cout << "\n================================= Borrowing a Laptop =================================\n";
        const bool ok = laptops.BorrowLaptop(currentUser);
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 3) {
        cout << "\n================================= Borrowing a Book =================================\n";
        books.BorrowBook(currentUser);
    } else if (choice == 4) {
        cout << "\n================================= Your Bookings =================================\n";
        const string uname = currentUser->getUsername();
        cout << "\nRooms:\n";
        rooms.showUserBookings(uname);
        cout << "\nLaptops:\n";
        laptops.showUserBookings(uname);
        cout << "\nBooks:\n";
        books.showUserBookings(uname);
    } else if (choice == 5) {
        cout << "Logging out...\n";
    } else {
        cout << "Invalid choice. Try again.\n";
    }
}

void LibrarySystem::handleAdminChoice(int choice) {
    if (choice == 1) {
        cout << "\n================================= Add Book =================================\n";
        books.addBookInteractive();
    } else if (choice == 2) {
        cout << "\n================================= Remove Book =================================\n";
        books.removeBookInteractive();
    } else if (choice == 3) {
        cout << "\n================================= Add Laptop =================================\n";
        laptops.addLaptopInteractive();
    } else if (choice == 4) {
        cout << "\n================================= Remove Laptop =================================\n";
        laptops.removeLaptopInteractive();
    } else if (choice == 5) {
        cout << "\n================================= Add Room =================================\n";
        rooms.addRoomInteractive();
    } else if (choice == 6) {
        cout << "\n================================= Remove Room =================================\n";
        rooms.removeRoomInteractive();
    } else if (choice == 7) {
        cout << "\n================================= Booking a Room (Admin) =================================\n";
        bool ok = rooms.bookRoom(currentUser);
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 8) {
        cout << "\n================================= Borrowing a Laptop (Admin) =================================\n";
        bool ok = laptops.BorrowLaptop(currentUser);
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 9) {
        cout << "\n================================= Borrowing a Book (Admin) =================================\n";
        books.BorrowBook(currentUser);
    } else if (choice == 10) {
        cout << "Logging out...\n";
    } else {
        cout << "Invalid choice. Try again.\n";
    }
}


void LibrarySystem::run() {
    bool running = true;

    while (running) {
        const bool continueProgram = loginLoop();
        if (!continueProgram) {
            running = false;
            break;
        }

        if (!currentUser) continue;

        userSession();
    }
}
