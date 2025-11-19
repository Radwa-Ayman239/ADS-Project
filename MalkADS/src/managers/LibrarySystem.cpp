//
// Created by hamdy on 11/19/2025.
//

#include "../../include/managers/LibrarySystem.h"
#include <iostream>
#include <string>
using namespace std;

LibrarySystem::LibrarySystem() {
    loadData();
}

void LibrarySystem::loadData() {
    users.loadUsersFromFile();
    rooms.loadRoomsFromFile();
}

void LibrarySystem::showUserMenu() {
    std::cout << "\n================================= User Menu =================================\n";
    std::cout << "\nMenu:\n";
    std::cout << "1. Book a room\n";
    std::cout << "2. Borrow a laptop\n";
    std::cout << "3. Borrow a book\n";
    std::cout << "4. LogOut\n";
    std::cout << "\nEnter your choice: ";
}

void LibrarySystem::showAdminMenu() {
    std::cout << "\n================================= Admin Menu =================================\n";
    std::cout << "\nMenu:\n";
    std::cout << "1. Add a new book\n";
    std::cout << "2. Remove a book\n";
    std::cout << "3. Add a laptop\n";
    std::cout << "4. Remove a laptop\n";
    std::cout << "5. Book a room (admin)\n";
    std::cout << "6. Borrow a laptop (admin)\n";
    std::cout << "7. Borrow a book (admin)\n";
    std::cout << "8. LogOut\n";
    std::cout << "\nEnter your choice: ";
}

bool LibrarySystem::loginLoop() {
    string username;
    string password;

    cout << "\n================================= Login Page =================================\n";
    cout << "Enter username (or type 'quit' to quit): ";
    cin >> username;

    if (username == "quit") {
        cout << "Exiting program.\n";
        return false;
    }

    cout << "Enter password: ";
    cin >> password;

    const User *loggedIn = users.login(username, password);
    if (!loggedIn) {
        cout << "Incorrect username or password. Try again.\n";
        return true;
    }

    currentUser = loggedIn;
    cout << "\nLogin successful! (" << (isCurrentUserAdmin() ? "admin" : "user") << ")\n";
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
            if (choice == 6) inSession = false;
        } else {
            handleUserChoice(choice);
            if (choice == 4) inSession = false;
        }

        if (inSession) {
            char again;
            cout << "\n\nDo you want to perform another action? (y or n): ";
            cin >> again;
            if (again == 'n' || again == 'N') {
                cout << "\nLogging out...\n";
                inSession = false;
            }
        }
    }

    currentUser = nullptr;
}

void LibrarySystem::handleUserChoice(const int choice) {
    if (choice == 1) {
        cout << "\n================================= Booking a Room =================================\n";
        const bool ok = rooms.bookRoom();
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 2) {
        cout << "\n================================= Borrowing a Laptop =================================\n";
        const bool ok = laptops.BorrowLaptop();
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 3) {
        cout << "\n================================= Borrowing a Book =================================\n";
        books.BorrowBook();
    } else if (choice == 4) {
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
        cout << "\n================================= Booking a Room (Admin) =================================\n";
        bool ok = rooms.bookRoom();
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 6) {
        cout << "\n================================= Borrowing a Laptop (Admin) =================================\n";
        bool ok = laptops.BorrowLaptop();
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 7) {
        cout << "\n================================= Borrowing a Book (Admin) =================================\n";
        books.BorrowBook();
    } else if (choice == 8) {
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

        userSession();
    }
}
