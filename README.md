# ADS-Project

## Priority Queue
A Priority Queue is a specialized data structure that stores elements in a way that allows the highest-priority element to be accessed first, regardless of the order they were added. Unlike a normal queue (FIFO), where the first element inserted is served first, a priority queue arranges elements by a defined priority rule — meaning insertion order does not determine service order. In this implementation, the priority queue is built using a binary heap, providing:
O(log n) insertion and deletion
O(1) access to the highest-priority element
## Main functions: 
1. PriorityQueue(int initialCapacity = 10):
Initializes an empty priority queue with a given initial capacity.
Allocates dynamic memory for the heap array and sets the current size to zero.
Used to create a new queue for each book to store its borrow requests.
Complexity: O(1)

2. ~PriorityQueue():
Releases all dynamically allocated memory and destroys the heap array when the queue object goes out of scope.
Prevents memory leaks and ensures clean termination.
Complexity: O(1)

3. void push(const T& value):
Inserts a new element into the priority queue.
If the array is full, the capacity is doubled using resize().
The new element is placed at the end of the heap and moved to its proper position using heapifyUp().
Ensures correct order based on the priority rule — earlier borrow intervals or non-repeating borrowers rise to the top.
Complexity: O(log n)

4. T top() const:
Returns the element with the highest priority without removing it from the queue.
Accesses the element at the root of the heap (heapArray[0]).
If the queue is empty, displays an error message and exits.
Complexity: O(1)

5. void pop():
Removes the highest-priority element from the queue.
Replaces the root element with the last element in the heap, decreases the size, and restores heap order using heapifyDown().
Ensures the next eligible borrower automatically moves to the top.
Complexity: O(log n)

6. bool isEmpty() const:
Checks whether the queue is empty.
Returns true if no elements are stored; otherwise, returns false.
Used to verify if any pending borrow requests exist for a specific book.
Complexity: O(1)

7. int size() const:
Returns the total number of elements currently stored in the queue.
Used to display or track how many requests are pending for a particular book.
Complexity: O(1)

8. void printHeap() const:
Prints all elements of the heap array in their current internal order.
Used mainly for visualization and debugging to observe how borrow requests are arranged by priority.
Complexity: O(n)

9. void resize():
Doubles the capacity of the heap array when it becomes full.
Creates a new array, copies existing elements, deletes the old array, and replaces it with the new one.
Ensures the queue can handle a growing number of requests efficiently.
Complexity: O(n)

10. void heapifyUp(int index):
Moves a newly added element upward in the heap to restore the correct order.
Compares the element with its parent and swaps them if it has higher priority.
Continues until the correct position is reached or the root is found.
Complexity: O(log n)

11. void heapifyDown(int index):
Rebalances the heap after removing the top element.
Compares the current node with its children and swaps it with the higher-priority child when necessary.
Repeats the process until the heap property is restored throughout the structure.
Complexity: O(log n)

### Purpose
In the context of the library booking system, the priority queues acts as follows: 
Each book in the library has its own priority queue that manages all active borrow requests for that specific title.
The queue ensures:
Requests are handled in the order they were submitted (standard queue logic).
Conflicts between overlapping requests are resolved fairly.
Students who borrowed the same book before cannot automatically monopolize it if another student requests it next. This is done by the
`Exception — Borrowing Conflict Rule`:
If a student who borrowed the same book previously submits a new request and another student requests it for an overlapping interval, the other student’s request receives higher priority, even if it was submitted later.
### Example visualization in flow form: 
📖 Book: “Data Structures and Algorithms”

1️⃣ Ali requests (April 10 – May 10)
   → Queue: [Ali]

2️⃣ Sara requests (April 5 – April 20)
   → Overlaps with Ali’s new request
   → Ali borrowed this book last month ⚠️
   → Rule triggered: Sara gains priority

Queue Reordered Automatically:
   [Sara, Ali]

3️⃣ System Processes:
   → top() → Sara
   → pop() → remove Sara after processing
   → top() → Ali (next in line)


## HashMap 

### Overview
The `HashMap` class is a core component of the Library Time Conflict Detection Tool, providing efficient storage and retrieval of library resources and their associated booking schedules. This generic hash table implementation uses separate chaining for collision resolution and is designed to manage various library resources such as devices, books, study rooms, and user information.

### Purpose
In the context of the library booking system, the HashMap serves two primary functions:
1. **Resource Management**: Maps resource identifiers (e.g., "StudyRoom-101", "Laptop-042", "Book-ISBN") and their corresponding interval trees that track booking time slots
2. **User Management**: Maps user identifiers to user profiles and their booking histories

### Architecture (in context of HashMap)
<pre>
┌─────────────────────────────────────────────────────────────────────────┐
│                    HashMap (Separate Chaining)                          │
└─────────────────────────────────────────────────────────────────────────┘
 Index →  Bucket (linked list)
 
 [0]  ──►  ┌───────────────────────────────────────────────────────────┐
           │   Resource: "StudyRoom-101"                               │
           │   Value   : IntervalTree *                                │
           │   ▼                                                       │
           │   Interval Tree (Bookings)                                │
           │   ┌─────────────────────────────────────────────────────┐ │
           │   │ [09:00,11:00] → user001                             │ │
           │   │ [13:00,15:00] → user002                             │ │
           │   └─────────────────────────────────────────────────────┘ │
           └──────┬────────────────────────────────────────────────────┘
                  │ next
                  ▼
           ┌───────────────────────────────────────────────────────────┐
           │   Resource: "Laptop-007"                                  │
           │   Value   : IntervalTree *                                │
           │   ▼                                                       │
           │   Interval Tree (Bookings)                                │
           │   ┌─────────────────────────────────────────────────────┐ │
           │   │ [08:30,10:00] → student023                          │ │
           │   │ [12:00,13:30] → student045                          │ │
           │   └─────────────────────────────────────────────────────┘ │
           └───────────────────────────────────────────────────────────┘

 [1]  ──►  (null)

 [2]  ──►  ┌───────────────────────────────────────────────────────────┐
           │   Resource: "Book-ISBN-978-..."                           │
           │   Value   : IntervalTree *                                │
           │   ▼                                                       │
           │   Interval Tree (Bookings)                                │
           │   ┌─────────────────────────────────────────────────────┐ │
           │   │ [10:00,12:00] → faculty_112                         │ │
           │   │ [14:30,16:00] → student219                          │ │
           │   └─────────────────────────────────────────────────────┘ │
           └───────────────────────────────────────────────────────────┘

 [3]  ──►  (null)

 ...
 
 Legend:
 - Each array cell is a bucket indexed by hash(key) % capacity
 - Collisions are handled via a singly linked list (separate chaining)
 - Each node stores:
     key   = resource ID (e.g., "StudyRoom-101", "Laptop-007")
     value = pointer to that resource’s IntervalTree (time bookings)
 - IntervalTree performs overlap checks and stores per-resource bookings
</pre>

### Key Features
#### 1. Constant-Time Operations
O(1) average lookup, insertion, and deletion  
Efficient for large-scale systems with thousands of resources

#### 2. Automatic Resizing
Automatically doubles capacity when the load factor exceeds 0.75

#### 3. Collision Handling
Separate chaining prevents data loss in hash collisions

#### 4. Type Safety
Template-based implementation enforces type safety at compile time

### API Reference

#### `bool putNew(const K& key, const V& value)`
Registers a new resource or user in the system, ignoring existing keys.

#### `V* get(const K& key) const`
Retrieves the value associated with a given key.

#### `bool erase(const K& key)`
Removes a specific resource or user entry.

#### `bool contains(const K& key) const`
Checks whether a key exists in the HashMap.

#### `void clear()`
Removes all key-value pairs from the HashMap but retains the bucket structure.

#### `int size() const`
Returns the number of entries in the HashMap.



**-Queue Documentation:**
Why We Used a Queue?

In our library scheduling system, we implemented a Queue data structure to manage multiple user commands efficiently, such as borrowing books, reserving laptops, and booking study rooms, all within a single program run.

A Queue follows the First-In-First-Out (FIFO) principle, meaning that commands are processed in the exact order they are received.
This guarantees fairness, order preservation, and smooth handling of sequential tasks, especially when users perform multiple actions at once.

**-Role of the Queue in Our System**

When a user enters several commands (e.g., borrow, book, cancel), each command is enqueued in order.
The system then dequeues and executes them one by one.
This design prevents command overlap, avoids scheduling collisions, and ensures that operations are handled consistently.

**Example:**
User commands: borrow → book → cancel <br>
Queue order:  [borrow] → [book] → [cancel]<br>
Execution:    borrow (done) → book (done) → cancel (done) <br>


**Why a Circular Dynamic Queue?**

**1-Circular structure:** avoids wasting memory after dequeuing elements.<br>
**2- Dynamic resizing:** automatically doubles in size when full.<br>


**Queue Flow Diagram**
    A[User Inputs Command] --> B[Enqueue Command into Queue]<br>
    B --> C{Is Queue Full?}<br>
    C -- Yes --> D[Resize Queue]<br>
    C -- No --> E[Keep Adding Commands]<br>
    E --> F[Dequeue Commands Sequentially]<br>
    F --> G[Execute Each Command (Borrow, Book, Cancel)]<br>
    G --> H[Display Updated Schedule]<br>

**What makes it special?**
-Handles multiple user commands efficiently in one session.<br>
-Preserves execution order and avoids task collisions.<br>
-Offers a fair, and memory-efficient command scheduler.<br>
