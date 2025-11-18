#include <iostream>
#include <string>

#include "user.cpp"
#include "user.h"

#include "users.h"
#include "users.cpp"

#include "book.h"
#include "book.cpp"

#include "rooms.h"
#include "rooms.cpp"
#include "laptops.h"
#include "laptops.cpp"
#include "books.h"
#include "books.cpp"

#include "IntervalTreeComplete.h"
#include "IntervalTreeComplete.cpp"

using namespace std;

int main() {
    Users users;
    Rooms rooms;
    Laptops laptops;
    Books books;

    users.loadUsersFromFile();
    rooms.loadRoomsFromFile();
    laptops.loadLaptopsFromFile();
    books.loadBooksFromFile();

    string username;
    string password;
    bool resultofBooking;

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
        cout << "\n================================= Menu =================================\n";

        bool keepUsingMenu = true;

        while (keepUsingMenu) {  
            int choice;
            cout << "\nMenu:\n";
            cout << "1. Book a room\n";
            cout << "2. Borrow a laptop\n";
            cout << "3. Borrow a book\n";
            cout << "4. LogOut\n";
            cout << "\nEnter your choice: ";
            cin >> choice;
            cout << "\n";

            if (choice == 1) {
                cout << "\n================================= Booking a Room =================================\n";
                resultofBooking = rooms.bookRoom();
                if (resultofBooking) cout << "\nRoom booked successfully!\n";
                else cout << "\nUnable to book room - conflict in scheduling\n";


            } else if (choice == 2) {
                cout << "\n================================= Borrowing a Laptop =================================\n";
                resultofBooking = laptops.BorrowLaptop();
                if (resultofBooking) {
                    cout << "\nFree Laptop available!\n"; 
                    cout << "\nGo to the library help desk at the start of booking period, give your details, and you will be given a laptop";
                }
                else cout << "Unable to borrow laptop at this time\n";


            } else if (choice == 3) {
                cout << "\n================================= Borrowing a Book =================================\n";
                cout << "You chose to borrow a book.\n";
                books.BorrowBook();

            } else if (choice == 4) {
                cout << "Logging out...\n";
                keepUsingMenu = false;   // Exit menu loop → return to login loop
                break;
            } else {
                cout << "Invalid choice. Try again.\n";
                continue;
            }

            // Ask if they want to do more
            char again;
            cout << "\n\nDo you want to book something else? (y or n): ";
            cin >> again;

            if (again == 'n' || again == 'N') {
                cout << "\nLogging out...\n";
                keepUsingMenu = false;   // return to login loop
            }
        }

    } else {
        cout << "Incorrect username or password. Try again.\n";
    }
}

return 0;
}