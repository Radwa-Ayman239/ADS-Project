//
// Created by hamdy on 10/17/2025.
//

#ifndef ADS_PROJECT_HASH_MAP_H
#define ADS_PROJECT_HASH_MAP_H
#include <string>
#include <utility>
using namespace std;

// Minimal separate-chaining hash map specialized for String -> void*.
// Used to map resource IDs to owned Interval Tree*.
// Independent of IntervalTree: it stores opaque pointers.

class HashMap
{
public:
    HashMap();
    ~HashMap();

    // Disallow copy for simplicity; can be added if needed
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

    // CRUD:keys
    bool putNew(const string&key, void*value);
    void* get(const string& key) const;
    bool erase(const string& key);
    bool contains(const string& key) const;
    void clear();
    int size() const { return size_; }

private:
    struct Node {
        string key;
        void* val;
        Node* next;
        Node(string  k, void* v) : key(std::move(k)), val(v), next(nullptr) {}
    };

    Node** buckets_;
    int capacity_;
    int size_;
    float max_load_;

    static unsigned long hash(const string& s) ;
    void rehash(int newCap);
    void ensureCapacity();
};


#endif //ADS_PROJECT_HASH_MAP_H