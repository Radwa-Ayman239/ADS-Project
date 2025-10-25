#ifndef QUEUE_H
#define QUEUE_H

#include <string>
using namespace std;

class queue{
    private:
    string * com; // array that will store the commands from the user
    int capacity, front, rear;
    int count;


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


    //queue();
    void enqueue( string);
    void dequeue();
    bool isEmpty() const;
    bool isFull() const;
    string Front() const;
    int size() const;

    //maybe we need displaying the queue
    void display() const;

    //destructor 
    ~queue() {
    delete[] com;
}

    
};





#endif