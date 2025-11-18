#ifndef QUEUE_H
#define QUEUE_H

#include <string>

using namespace std;

class queue{
    private:
    string * com; 
    int capacity, front, rear;
    int count; // to keep track of the number if elements stored in the queue
    void resize(); //when the array is full 



    public:

    // constructor
    queue(){
        capacity = 10;
        com = new string [capacity];
        front = 0;
        rear = -1;
        count = 0; 
    }



    void enqueue(const string&);
    void dequeue();
    bool isEmpty() const;
    bool isFull() const;
    string Front() const;
    int size() const;
    int getCapacity() const; 
    void display() const;
    void clear();

    //destructor 
    ~queue() {
    delete[] com;
}

    
};





#endif
