//
// Created by sedra on 12/11/2025.
//

#include <iostream>
#include "include/models/User.h"

#include "include/managers/UserManager.h"
#include "include/queue.h"


using namespace std;

int main() {
    UserManager userManager;

    cout << "=== Welcome to Our Library ===\n";
    cout << endl << endl;

    User* currentUser = nullptr;

    while (!currentUser) {
        cout << "---- choose an option ----";
        cout << "1. Login\n2. Signup\n ";
        int choice;
        cin >> choice;
        cin.ignore(); // ignore leftover newline

        if (choice == 1) currentUser = userManager.login();
        else if (choice == 2) currentUser = userManager.signup();
        else cout << "Invalid option. Try again.\n";
    }

    cout << "\n--- Welcome " << currentUser->getUsername() << " ---\n";


    queue commandQueue;

    bool running = true;
    while (running) {
        cout << "\nEnter commands (borrowBook, borrowLaptop, bookRoom) or 'exit' to quit:\n";

        string cmd;
        getline(cin, cmd);

        //    first command entered is the exit
        if (cmd == "exit") {
            running = false;
            break;
        }

        commandQueue.enqueue(cmd);

        // Process all commands in the queue
        while (!commandQueue.isEmpty()) {
            string command = commandQueue.Front();
            commandQueue.dequeue();

            if (command == "borrowBook") {
                /*---------This is just for now untill we implement the remaining classes-----------*/

                userManager.notify(currentUser, "Book borrowed successfully");
            } else if (command == "borrowLaptop") {

               userManager.notify(currentUser, "Laptop borrowed successfully");
            } else if (command == "bookRoom") {

                userManager.notify(currentUser, "Room booked successfully");
            } else {

                userManager.notify(currentUser, "Unknown command attempted: " + command);
            }
        }

        // Display all notifications
        userManager.allNotifications(currentUser);
    }

    cout << "Goodbye! Come again soon.....\n";
    return 0;
}
