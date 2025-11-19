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
}

void LibrarySystem::showUserMenu() {
    std::cout << "\n================================= User Menu =================================\n";
    std::cout << "\nMenu:\n";
    std::cout << "1. Book a room\n";
    std::cout << "2. Borrow a laptop\n";
    std::cout << "3. Borrow a book\n";
    std::cout << "4. View my bookings\n";
    std::cout << "5. LogOut\n";
    std::cout << "\nEnter your choice: ";
}

void LibrarySystem::showAdminMenu() {
    cout << "\n================================= Admin Menu =================================\n";
    cout << "\nMenu:\n";
    cout << "1. Add a new book\n";
    cout << "2. Remove a book\n";
    cout << "3. Add a laptop\n";
    cout << "4. Remove a laptop\n";
    cout << "5. Add a room\n";
    cout << "6. Remove a room\n";
    cout << "7. Book a room (admin)\n";
    cout << "8. Borrow a laptop (admin)\n";
    cout << "9. Borrow a book (admin)\n";
    cout << "10. LogOut\n";
    cout << "\nEnter your choice: ";
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
            if (choice == 10) inSession = false;
        } else {
            handleUserChoice(choice);
            if (choice == 5) inSession = false;
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
        const bool ok = rooms.bookRoom(currentUser->getUsername());
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 2) {
        cout << "\n================================= Borrowing a Laptop =================================\n";
        const bool ok = laptops.BorrowLaptop(currentUser->getUsername());
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 3) {
        cout << "\n================================= Borrowing a Book =================================\n";
        books.BorrowBook(currentUser->getUsername());
    } else if (choice == 4) {
        cout << "\n================================= Your Bookings =================================\n";
        const string uname = currentUser->getUsername();
        cout << "\nRooms:\n";
        rooms.showUserBookings(uname);
        cout << "\nLaptops:\n";
        laptops.showUserBookings(uname);
        cout << "\nBooks:\n";
        books.showUserBookings(uname);
    }
    else if (choice == 5) {
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
        bool ok = rooms.bookRoom(currentUser->getUsername());
        if (ok) cout << "\nRoom booked successfully!\n";
        else cout << "\nUnable to book room - conflict in scheduling\n";
    } else if (choice == 8) {
        cout << "\n================================= Borrowing a Laptop (Admin) =================================\n";
        bool ok = laptops.BorrowLaptop(currentUser->getUsername());
        if (ok) {
            cout << "\nFree Laptop available!\n";
            cout <<
                    "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
        } else {
            cout << "Unable to borrow laptop at this time\n";
        }
    } else if (choice == 9) {
        cout << "\n================================= Borrowing a Book (Admin) =================================\n";
        books.BorrowBook(currentUser->getUsername());
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

        userSession();
    }
}
