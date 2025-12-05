#ifndef BOOK_H
#define BOOK_H
#include <string>
using namespace std;

class Book {
private:
  string bookid;
  string title;
  string author;

public:
  Book();

  Book(const string &bookid, const string &title, const string &author);

  string getID();

  string getAuthor();

  string getTitle();
};

#endif
