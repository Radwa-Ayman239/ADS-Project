# include "queue.h"

#include <iostream>
using namespace std;

/* queue :: queue(){
        capacity = 10;
        com = new string [capacity];
        front = 0;
        rear = -1;
        count = 0; 
    } */

void queue::enqueue (string comm){

    cout << "rear=" << rear << " capacity=" << capacity << " count=" << count << endl;
if (isFull()){
    resize();
}

 rear = (rear + 1) % capacity; 
 com[rear] = comm;                 
        
    count++;                         
}

void queue:: dequeue(){
    if (isEmpty()){
     cout <<"The schedule is already empty"<<endl;
     return;
    }


     front = (front +1) % capacity;
     count --;
     
}

bool queue:: isEmpty() const{
    return count ==0;

}

bool queue:: isFull() const{
    return count == capacity;

}

void queue:: display() const{

     if (isEmpty()) {
        cout << "Schedule is empty." << endl;
        return;
    }

    cout << "Schedule: ";
    for (int i = 0; i < count; i++) {
        cout << com[(front + i) % capacity] << " ";
    }
    cout << endl;


}

string queue :: Front() const{
    return com[front]; 
}

int queue:: size() const {
    return count;
}

void queue::resize() {
    int oldCapacity = capacity;
    capacity = capacity * 2;
    string* newarray = new string[capacity];

    for (int i = 0; i < count; i++) {
        newarray[i] = com[(front + i) % oldCapacity];
    }

    delete[] com;
    com = newarray;

    front = 0;
    rear = count;
}


