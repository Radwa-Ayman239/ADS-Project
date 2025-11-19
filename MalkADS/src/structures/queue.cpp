//
// Created by hamdy on 11/19/2025.
//
#include "../../include/structures/queue.h"
#include <iostream>
using namespace std;


void queue::enqueue(const string &comm) {
    if (isFull()) {
        resize();
    }

    rear = (rear + 1) % capacity;
    com[rear] = comm;

    count++;
}

void queue::dequeue() {
    if (isEmpty()) {
        cout << "The schedule is already empty" << endl;
        return;
    }


    front = (front + 1) % capacity;
    count--;
}

bool queue::isEmpty() const {
    return count == 0;
}

bool queue::isFull() const {
    return count == capacity;
}

void queue::display() const {
    if (isEmpty()) {
        cout << "No commands founded." << endl;
        return;
    }

    cout << "User Commands: ";
    for (int i = 0; i < count; i++) {
        cout << com[(front + i) % capacity] << " ";
    }
    cout << endl;
}

string queue::Front() const {
    if (isEmpty()) {
        cout << "No Commands founded";
        return "";
    }
    return com[front];
}

int queue::size() const {
    return count; // total elements in the queue
}

int queue::getCapacity() const {
    return capacity; // total available space
}


//This function is only in case the number of commands used in one time exceeded 10 (the capacity)
void queue::resize() {
    int oldCapacity = capacity;
    capacity = capacity * 2;
    string *newarray = new string[capacity];

    for (int i = 0; i < count; i++) {
        newarray[i] = com[(front + i) % oldCapacity];
    }

    delete[] com;
    com = newarray;

    front = 0;
    rear = count - 1;
}

void queue::clear() {
    front = 0;
    rear = -1;
    count = 0;
}
