#include "books.h"
//#include "book.h"
#include <fstream>
#include <iostream>
using namespace std;

Books::Books() {}



Books::~Books() {
    //Data Structure Change
    unordered_map<string, RedBlackIntervalTree*>::iterator it;

    for (it = BookTable.begin(); it != BookTable.end(); ++it) {
        delete it->second; 
    }

    BookTable.clear();
}


void Books::loadBooksFromFile() {
    ifstream file("books.txt");
    if (!file) {
        cout << "Error opening books.txt\n";
        return;
    }

    string line;
    string bookID;
    string bookTitle;
    string bookAuthor;

    int commaPos1;
    int commaPos2;

    while (getline(file, line)) {
        commaPos1 = line.find(',');
        if (commaPos1 != string::npos) {
            commaPos2 = line.find(',', commaPos1 + 1);

            if (commaPos2 != string::npos) {
                bookID = line.substr(0, commaPos1);
                bookTitle = line.substr(commaPos1 + 1, commaPos2 - (commaPos1 + 1));
                bookAuthor = line.substr(commaPos2 + 1);
            }
        }

        Book newBook = Book(bookID, bookTitle, bookAuthor);
        //Data Structure Change
        ID_To_BookTable[bookID] = newBook;

        RedBlackIntervalTree* tree = new RedBlackIntervalTree();
        //Data Structure Change
        BookTable[bookID] = tree;
       
    }

    file.close();
}


void Books::BorrowBook() {
    int searchmethod;
    cout << "\nYou can search for a book either \n\n1. By Title \nor \n2. By Author";
    cout << "\n\nHow would you like to search?: "; cin>> searchmethod;

    //Data Structure Change
    unordered_map<string, Book>::iterator it;
    string titlesearch = "";
    string currentTitle;
    string foundbookID;
    bool bookfound;


    if (searchmethod == 1) {
        currentTitle = "";
        foundbookID = "";
        bookfound = false;

        cout << "\nEnter the title of the book you are searching for: "; cin.ignore(); getline(cin, titlesearch);

        for (int i = 0; i < titlesearch.length(); i++) {
            titlesearch[i] = tolower(titlesearch[i]);
        }

        //Data Structure Change
        for (it = ID_To_BookTable.begin(); it != ID_To_BookTable.end(); ++it) {
            
            currentTitle = it->second.getTitle();

            for (int i = 0; i < currentTitle.length(); i++) {
                currentTitle[i] = tolower(currentTitle[i]);
            }

            if (currentTitle == titlesearch) {
                bookfound = true;
                foundbookID = it->second.getID();
                break;
            }
        }

        if (bookfound == false) {
            cout << "\nSorry the library does not have this book";
        }
        else {
            int startperiod;
            int endperiod;
            cout << "\n\nWhen would you like to borrow this book?";
            cout << "\n\nStart of borrowing period: "; cin >> startperiod;
            cout << "End of borrowing period: "; cin >> endperiod;


            //Data Structure Change
            if (BookTable[foundbookID]->searchOverlap(startperiod, endperiod, true) == false) {
                BookTable[foundbookID]->insert(startperiod, endperiod);
                cout << "\n\nBooking successful!";
                cout << "\nGo to the library help desk at the start of booking period, give your details, and you will be given your desired book";
            }
            else {
                cout << "\nUnable to borrow this book during the desired period";
            }
        }


    }
    else {
        string searchAuthor = "" ;
        string currentAuthor = "";
        bool authorfound = false;

        currentTitle = "";
        foundbookID = "";
        bookfound = false;


        cout << "\nEnter the author you are searching for: "; cin.ignore(); getline(cin, searchAuthor);

        for (int i = 0; i < searchAuthor.length(); i++) {
            searchAuthor[i] = tolower(searchAuthor[i]);
        }

        
        //Data Structure Change
        for (it = ID_To_BookTable.begin(); it != ID_To_BookTable.end(); ++it) {
            
            currentAuthor = it->second.getAuthor();

            for (int i = 0; i < currentAuthor.length(); i++) {
                currentAuthor[i] = tolower(currentAuthor[i]);
            }

            if (currentAuthor == searchAuthor) {
                authorfound = true;
                break;
            }
        }

        if (authorfound == false) {
            cout << "\nSorry we do not have any books by this author";
        }
        else {
            cout << "\nHere are the books we have written by this author: " << endl;

            //Data Structure Change
            for (it = ID_To_BookTable.begin(); it != ID_To_BookTable.end(); ++it) {
                
                currentAuthor = it->second.getAuthor();

                for (int i = 0; i < currentAuthor.length(); i++) {
                    currentAuthor[i] = tolower(currentAuthor[i]);
                }
                
                if (currentAuthor == searchAuthor) {
                    cout << "- " << it->second.getTitle() <<endl;
                }
            }

            cout << "\nEnter the title of the book you are searching for: "; getline(cin, titlesearch);
            string author_of_inputbook = "";

            for (int i = 0; i < titlesearch.length(); i++) {
                titlesearch[i] = tolower(titlesearch[i]);
            }
    
            //Data Structure Change
            for (it = ID_To_BookTable.begin(); it != ID_To_BookTable.end(); ++it) {
                
                currentTitle = it->second.getTitle();
    
                for (int i = 0; i < currentTitle.length(); i++) {
                    currentTitle[i] = tolower(currentTitle[i]);
                }
    
                if (currentTitle == titlesearch) {
                    bookfound = true;
                    foundbookID = it->second.getID();
                    author_of_inputbook = it->second.getAuthor();
                    break;
                }
            }
    

            bool authormatch = false;

            for (int i = 0; i < author_of_inputbook.length(); i++) {
                author_of_inputbook[i] = tolower(author_of_inputbook[i]);
            }

            if (author_of_inputbook == searchAuthor) {
                authormatch = true;
            }

            if (bookfound == false) {
                cout << "\nSorry the library does not have this book";
            }
            else {
                char stillwant;
                if (authormatch == false) {
                    cout << "\nThe author of " << titlesearch << " is " << author_of_inputbook;
                    cout << "\nThis is not the author you were originally looking for ";
                    cout << "\nDo you still want the book? (y or n): "; cin>> stillwant; 
                }

                if (authormatch == true || stillwant == 'y' || stillwant == 'Y') {
                    int startperiod;
                    int endperiod;
                    cout << "\n\nWhen would you like to borrow this book?";
                    cout << "\n\nStart of borrowing period: "; cin >> startperiod;
                    cout << "End of borrowing period: "; cin >> endperiod;
    
                    //Data Structure Change
                    if (BookTable[foundbookID]->searchOverlap(startperiod, endperiod, true) == false) {
                        BookTable[foundbookID]->insert(startperiod, endperiod);
                        cout << "\n\nBooking successful!";
                        cout << "\nGo to the library help desk at the start of booking period, give your details, and you will be given your desired book";
                    }
                    else {
                        cout << "\nUnable to borrow this book during the desired period";
                    }
                }
            }
                    
        }
 
    }
    

}








//Borrow a book
//-Asks if user wants to search by author, or search by name - call respective function
//-Ask for interval 



//Search by bookname

//Search by author 

