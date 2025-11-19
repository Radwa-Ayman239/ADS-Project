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
    laptops.loadLaptopsFromFile();
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
    std::cout << "3. Book a room (admin)\n";
    std::cout << "4. Borrow a laptop (admin)\n";
    std::cout << "5. Borrow a book (admin)\n";
    std::cout << "6. LogOut\n";
    std::cout << "\nEnter your choice: ";
}

void LibrarySystem::run() {
    std::string username;
    std::string password;

    bool loginLoop = true;
    while (loginLoop) {
        std::cout << "\n================================= Login Page =================================\n";
        std::cout << "Enter username (or type 'quit' to quit): ";
        std::cin >> username;

        if (username == "quit") {
            std::cout << "Exiting program.\n";
            loginLoop = false;
            continue;
        }

        std::cout << "Enter password: ";
        std::cin >> password;

        const User *loggedIn = users.login(username, password);
        if (!loggedIn) {
            std::cout << "Incorrect username or password. Try again.\n";
            continue;
        }

        bool isAdmin = loggedIn->getIsAdmin();
        std::cout << "\nLogin successful! (" << (isAdmin ? "admin" : "user") << ")\n";

        bool inSession = true;
        while (inSession) {
            int choice;

            if (isAdmin) {
                showAdminMenu();
            } else {
                showUserMenu();
            }

            std::cin >> choice;
            std::cout << "\n";

            if (!isAdmin) {
                // regular user actions
                if (choice == 1) {
                    std::cout <<
                            "\n================================= Booking a Room =================================\n";
                    bool ok = rooms.bookRoom();
                    if (ok) std::cout << "\nRoom booked successfully!\n";
                    else std::cout << "\nUnable to book room - conflict in scheduling\n";
                } else if (choice == 2) {
                    std::cout <<
                            "\n================================= Borrowing a Laptop =================================\n";
                    bool ok = laptops.BorrowLaptop();
                    if (ok) {
                        std::cout << "\nFree Laptop available!\n";
                        std::cout <<
                                "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
                    } else {
                        std::cout << "Unable to borrow laptop at this time\n";
                    }
                } else if (choice == 3) {
                    std::cout <<
                            "\n================================= Borrowing a Book =================================\n";
                    books.BorrowBook();
                } else if (choice == 4) {
                    std::cout << "Logging out...\n";
                    inSession = false;
                } else {
                    std::cout << "Invalid choice. Try again.\n";
                }
            } else {
                // admin actions
                if (choice == 1) {
                    std::cout << "\n================================= Add Book =================================\n";
                    books.addBookInteractive();
                } else if (choice == 2) {
                    std::cout << "\n================================= Remove Book =================================\n";
                    books.removeBookInteractive();
                } else if (choice == 3) {
                    std::cout <<
                            "\n================================= Booking a Room (Admin) =================================\n";
                    bool ok = rooms.bookRoom();
                    if (ok) std::cout << "\nRoom booked successfully!\n";
                    else std::cout << "\nUnable to book room - conflict in scheduling\n";
                } else if (choice == 4) {
                    std::cout <<
                            "\n================================= Borrowing a Laptop (Admin) =================================\n";
                    bool ok = laptops.BorrowLaptop();
                    if (ok) {
                        std::cout << "\nFree Laptop available!\n";
                        std::cout <<
                                "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
                    } else {
                        std::cout << "Unable to borrow laptop at this time\n";
                    }
                } else if (choice == 5) {
                    std::cout <<
                            "\n================================= Borrowing a Book (Admin) =================================\n";
                    books.BorrowBook();
                } else if (choice == 6) {
                    std::cout << "Logging out...\n";
                    inSession = false;
                } else {
                    std::cout << "Invalid choice. Try again.\n";
                }
            }

            if (inSession) {
                char again;
                std::cout << "\n\nDo you want to perform another action? (y or n): ";
                std::cin >> again;
                if (again == 'n' || again == 'N') {
                    std::cout << "\nLogging out...\n";
                    inSession = false;
                }
            }
        }
    }
}
