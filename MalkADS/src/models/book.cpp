#include "../../include/models/book.h"
using namespace std;

Book::Book() {
    bookid = "";
    author = "";
    title = "";
}

Book::Book(const string& ID, const string& Title, const string& Author) {
    bookid = ID;
    title = Title;
    author = Author;
}



string Book::getID() {
    return bookid;
}

string Book::getAuthor() {
    return author;
}

string Book::getTitle() {
    return title;
}
