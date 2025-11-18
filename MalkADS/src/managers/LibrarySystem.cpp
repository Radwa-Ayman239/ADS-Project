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
    books.loadBooksFromFile();
}

void LibrarySystem::showMenu() {
    cout << "\n================================= Menu =================================\n";
    cout << "\nMenu:\n";
    cout << "1. Book a room\n";
    cout << "2. Borrow a laptop\n";
    cout << "3. Borrow a book\n";
    cout << "4. LogOut\n";
    cout << "\nEnter your choice: ";
}

void LibrarySystem::run() {
    string username;
    string password;

    bool login = true;
    while (login) {
        cout << "\n================================= Login Page =================================\n";
        cout << "Enter username (or type 'quit' to quit): ";
        cin >> username;

        if (username == "quit") {
            cout << "Exiting program.\n";
            login = false;
            continue;
        }

        cout << "Enter password: ";
        cin >> password;

        if (users.login(username, password)) {
            cout << "\nLogin successful!\n";

            bool keepUsingMenu = true;
            while (keepUsingMenu) {
                int choice;
                showMenu();
                cin >> choice;
                cout << "\n";

                bool resultofBooking = false;

                if (choice == 1) {
                    cout << "\n================================= Booking a Room =================================\n";
                    resultofBooking = rooms.bookRoom();
                    if (resultofBooking)
                        cout << "\nRoom booked successfully!\n";
                    else
                        cout << "\nUnable to book room - conflict in scheduling\n";
                } else if (choice == 2) {
                    cout <<
                            "\n================================= Borrowing a Laptop =================================\n";
                    resultofBooking = laptops.BorrowLaptop();
                    if (resultofBooking) {
                        cout << "\nFree Laptop available!\n";
                        cout <<
                                "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
                    } else {
                        cout << "Unable to borrow laptop at this time\n";
                    }
                } else if (choice == 3) {
                    cout << "\n================================= Borrowing a Book =================================\n";
                    cout << "You chose to borrow a book.\n";
                    books.BorrowBook();
                } else if (choice == 4) {
                    cout << "Logging out...\n";
                    keepUsingMenu = false;
                    break;
                } else {
                    cout << "Invalid choice. Try again.\n";
                    continue;
                }

                char again;
                cout << "\n\nDo you want to book something else? (y or n): ";
                cin >> again;

                if (again == 'n' || again == 'N') {
                    cout << "\nLogging out...\n";
                    keepUsingMenu = false;
                }
            }
        } else {
            cout << "Incorrect username or password. Try again.\n";
        }
    }
}
