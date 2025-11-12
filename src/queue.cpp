#include "queue.h"
#include <iostream>
using namespace std;


void queue::enqueue (const string& comm){

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
    return count ==0 ;

}

bool queue:: isFull() const{
     return count == capacity;

}

void queue:: display() const{

     if (isEmpty()) {
        cout << "No commands founded." << endl;
        return;
    }

    cout << "User Commands: ";
    for (int i = 0; i < count; i++) {
        cout << com[(front + i) % capacity] <<" ";
    }
    cout << endl;


}

string queue :: Front() const{
    if (isEmpty()){
        cout<<"No Commands founded";
    return "" ;
    }
    return com[front]; 
}

int queue:: size() const {
    return count; // total elements in the queue
}

int queue::getCapacity() const {
    return capacity;     // total available space
}


//This function is only in case the number of commands used in one time exceeded 10 (the capacity) 
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
    rear = count - 1 ;
}

void queue:: clear() {
    front= 0;
    rear = -1;
    count = 0;

}

int main() {
    queue q;

    cout << "Testing queue implementation...\n";

    // Test 1: Enqueue a few commands
    cout << "\n--- Enqueueing commands ---\n";
    q.enqueue("Borrow Laptop");
    q.enqueue("Book studyRoom");
    q.enqueue("Borrow Book");
    q.display();

    // Test 2: Check front and size
    cout << "\nFront element: " << q.Front() << endl;
    cout << "Current size: " << q.getCapacity() << endl;
    cout<<"The actual number of elements in the queue: "<<q.size()<<endl;

    // Test 3: Dequeue a command
    cout << "\n--- Dequeueing one command ---\n";
    q.dequeue();
    q.display();

    // Test 4: Enqueue more items (test wrapping around)
    cout << "\n--- Enqueueing more to test wrap-around ---\n";
    for (int i = 1; i <= 10; i++) {
        q.enqueue("Command " + to_string(i+2));
    }
    q.display();

    // Test 5: Force resize
    cout << "\n--- Forcing resize ---\n";
    //q.enqueue("Extra Command");
    //q.display();
    cout << "New capacity after resize: " << q.getCapacity() << endl;
    cout<<"The actual number of elements in the queue: "<<q.size()<<endl;

   

}

