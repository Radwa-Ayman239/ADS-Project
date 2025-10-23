#include "Priority_Queue.h"
using namespace std;
struct MaxComparator {
    bool operator()(const int& a, const int& b) const {
        return a > b;
    }
};
int main() {
    PriorityQueue<int> minHeap;
    minHeap.push(50);
    minHeap.push(30);
    minHeap.push(20);
    minHeap.push(40);
    minHeap.push(10);

    minHeap.printHeap();
    cout << "Top element: " << minHeap.top() << endl;
    minHeap.pop();
    minHeap.printHeap();

    // maxheap example
    PriorityQueue<int, MaxComparator> maxHeap;
    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(5);
    maxHeap.push(40);

    maxHeap.printHeap();
    cout << "Top element: " << maxHeap.top() << endl;

    return 0;
}
