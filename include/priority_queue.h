#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>
using namespace std;

template <typename T>
struct DefaultComparator {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};


template <typename T, typename Comparator = DefaultComparator<T>>
class PriorityQueue {
private:
    T* heapArray;
    int capacity;
    int currentSize;
    Comparator comp;

    void resize();
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    PriorityQueue(int initialCapacity = 10);
    ~PriorityQueue();

    bool isEmpty() const;
    int size() const;
    void push(const T& value);
    T top() const;
    void pop();
    void printHeap() const;
};


template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue(int initialCapacity) {
    capacity = initialCapacity;
    heapArray = new T[capacity];
    currentSize = 0;
}


template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::~PriorityQueue() {
    delete[] heapArray;
}


template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::resize() {
    int newCapacity = capacity * 2;
    T* newArray = new T[newCapacity];
    for (int i = 0; i < currentSize; i++)
        newArray[i] = heapArray[i];
    delete[] heapArray;
    heapArray = newArray;
    capacity = newCapacity;
}


template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (comp(heapArray[index], heapArray[parent])) {
            swap(heapArray[index], heapArray[parent]);
            index = parent;
        } else break;
    }
}


template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int target = index;

        if (left < currentSize && comp(heapArray[left], heapArray[target]))
            target = left;
        if (right < currentSize && comp(heapArray[right], heapArray[target]))
            target = right;

        if (target == index)
            break;

        swap(heapArray[index], heapArray[target]);
        index = target;
    }
}

// Check empty
template <typename T, typename Comparator>
bool PriorityQueue<T, Comparator>::isEmpty() const {
    return currentSize == 0;
}

// Size
template <typename T, typename Comparator>
int PriorityQueue<T, Comparator>::size() const {
    return currentSize;
}

// Push
template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T& value) {
    if (currentSize == capacity)
        resize();
    heapArray[currentSize] = value;
    heapifyUp(currentSize);
    currentSize++;
}

// Top
template <typename T, typename Comparator>
T PriorityQueue<T, Comparator>::top() const {
    if (isEmpty()) {
        cerr << "PriorityQueue is empty!\n";
        exit(EXIT_FAILURE);
    }
    return heapArray[0];
}

// Pop
template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::pop() {
    if (isEmpty()) {
        cerr << "PriorityQueue is empty!\n";
        return;
    }
    heapArray[0] = heapArray[currentSize - 1];
    currentSize--;
    heapifyDown(0);
}

// Print
template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::printHeap() const {
    cout << "Heap elements: ";
    for (int i = 0; i < currentSize; i++)
        cout << heapArray[i] << " ";
    cout << endl;
}

#endif // PRIORITY_QUEUE_H
