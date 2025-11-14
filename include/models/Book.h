//
// Created by hamdy on 11/14/2025.
//

#ifndef ADS_PROJECT_BOOK_H
#define ADS_PROJECT_BOOK_H

#include <string>
using namespace std;

struct Day {
    int value;
};

struct Loan {
    string username;
    Day start{};
    Day due{};
    bool active{};
};

struct Hold {
    string username;
    Day start{};
    Day end{};
    bool active{};
};

class Book {
private:
    string id_; // ISBN
    string title_;

    bool onLoan_;
    Loan currentLoan_;

    bool onHold_;
    Hold currentHold_;

public:
    Book(string id, string title) : id_(std::move(id)), title_(std::move(title)), onLoan_(false), currentLoan_(),
                                    onHold_(false),
                                    currentHold_() {
    }

    const string &getId() const { return id_; }
    const string &getTitle() const { return title_; }

    bool isOnLoan() const { return onLoan_; }
    bool isOnHold() const { return onHold_; }

    const Loan &getLoan() const { return currentLoan_; }
    const Hold &getHold() const { return currentHold_; }

    void startLoan(const string &username, const Day start, Day due) {
        onLoan_ = true;
        currentLoan_.username = username;
        currentLoan_.start = start;
        currentLoan_.due = due;
        currentLoan_.active = true;
    }

    void endLoan() {
        onLoan_ = false;
        currentLoan_.active = false;
    }

    void startHold(const string &username, Day start, Day end) {
        onHold_ = true;
        currentHold_.username = username;
        currentHold_.start = start;
        currentHold_.end = end;
        currentHold_.active = true;
    }

    void endHold() {
        onHold_ = false;
        currentLoan_.active = false;
    }
};


#endif //ADS_PROJECT_BOOK_H
