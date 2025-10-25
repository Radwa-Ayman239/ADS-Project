# ADS-Project

## Priority Queue
Documentation of the use of priority queues: 
In this project, a priority queue is used to manage book borrowing requests in situations where time conflicts occur between multiple users.
Consider the following scenario:
A student borrows a book for a month and then returns it. If the same student tries to borrow the same book again the following month, they will receive a notification stating:
“Same book borrowed the month before! Please note that if another student borrows it before your requested start date, your borrow request will be cancelled.” This situation introduces a potential borrowing conflict — two users requesting the same book for overlapping or consecutive time periods. To handle this efficiently, all borrow requests for a specific book are placed in a priority queue based on their priority level rather than the order of submission. If another user submits a borrow request before the start date of the first user’s requested borrowing interval, that user is assigned a higher priority in the queue. Even if the first user submitted their request earlier, the second user (with the earlier borrowing period) takes precedence because their request conflicts directly with the start of the first user’s interval.

---

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
