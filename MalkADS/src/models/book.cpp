#include "../../include/models/book.h"
#include <cctype>
using namespace std;

Book::Book() {
  bookid = "";
  author = "";
  title = "";
}

Book::Book(const string &ID, const string &Title, const string &Author) {
  bookid = ID;
  title = Title;
  author = Author;
}

string Book::getID() { return bookid; }

string Book::getAuthor() { return author; }

string Book::getTitle() {
  string result = title;
  bool newWord = true;
  for (size_t i = 0; i < result.length(); ++i) {
    if (isspace(result[i])) {
      newWord = true;
    } else {
      if (newWord) {
        result[i] = toupper(result[i]);
        newWord = false;
      } else {
        result[i] = tolower(result[i]);
      }
    }
  }
  return result;
}
