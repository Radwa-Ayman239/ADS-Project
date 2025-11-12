#include "Priority_Queue.h"
using namespace std;

struct MaxComparator {
    bool operator()(const int &a, const int &b) const {
        return a > b;
    }
};
