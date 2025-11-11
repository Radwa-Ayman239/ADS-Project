#ifndef ADS_PROJECT_HASH_MAP_H
#define ADS_PROJECT_HASH_MAP_H

#include <string>
#include <utility>
#include <functional>

/**
 * @file hash_map.h
 * @brief Template-based hash map implementation using separate chaining for collision resolution
 *
 * @tparam K Key type (must support std::hash and equality comparison)
 * @tparam V Value type
 */

/**
 * @class HashMap
 * @brief A generic hash map implementation using separate chaining
 *
 * This class implements a hash table with separate chaining collision resolution.
 * It provides O(1) average-case complexity for insertion, lookup, and deletion operations.
 * The hash map automatically resizes when the load factor exceeds a threshold (default 0.75).
 *
 * @tparam K The Key type - must be hashable via std::hash and support equality comparison
 * @tparam V the Value type - can be any type including pointers
 *
 * Features:
 * - Automatic resizing when load factor exceeds threshold
 * - Separate chaining for collision resolution
 * - Copy constructor and assignment operator deleted for safety
 *
 * Example Usage:
 * @code
 *  HashMap<std::string, int> map;
 *  map.putNew("key1", 42);
 *  int* val = map.get("key1");
 *  if (val) {
 *      std::cout << *val << std::endl;
 *  }
 * @endcode
 */
template<typename K, typename V>
class HashMap {
public:
    /**
     * @brief Default constructor
     *
     * Initializes the hash map with default capacity of 16 buckets
     * and a maximum load factor of 0.75.
     *
     * Time Complexity: O(n) where n is initial capacity
     * Space Complexity: O(n)
     */
    HashMap();

    /**
     * @brief Destructor
     *
     * Clears all entries and deallocates memory for buckets.
     * Note: Does not deallocate memory pointed to by stored pointers.
     *
     * Time Complexity: O(n) where n is the number of elements
     */
    ~HashMap();

    /**
     * @brief Copy Constructor (deleted)
     *
     * Copying is disabled to prevent issues with pointer ownership.
     * If needed, implement deep copy logic.
     */
    HashMap(const HashMap &) = delete;

    /**
     * @brief Copy assignment (deleted)
     *
     * Copying is disabled to prevent issues with pointer ownership.
     * If needed, implement deep copy logic.
     */
    HashMap &operator=(const HashMap &) = delete;

    /**
     * @brief Insert a new key-value pair
     *
     * Inserts a new entry only if the key does not already exist.
     * If insertion causes load factor to exceed threshold, triggers rehashing.
     *
     * @param key The key to insert
     * @param value The value to associate with the key
     * @return true if insertion succeeded, false if key already exists
     *
     * Time Complexity: O(1) average case, O(n) worst case (during rehash)
     * Space Complexity: O(1)
     */
    bool putNew(const K &key, const V &value);

    /**
     * @brief Retrieve value associated with a key
     *
     * Looks up the value for the given key.
     *
     * @param key The key to search for
     * @return Pointer to the value if found, nullptr otherwise
     *
     * Time Complexity: O(1) average case, O(n) worst case
     * Space Complexity: O(1)
     */
    V *get(const K &key) const;

    /**
     * @brief Remove a key-value pair
     *
     * Removes the entry with the specified key if it exists.
     *
     * @param key The key to remove
     * @return true if key was found and removed, false otherwise
     *
     * Time Complexity: O(1) average case, O(n) worst case
     * Space Complexity: O(1)
     */
    bool erase(const K &key);

    /**
     * @brief Check if a key exists
     *
     * @param key The key to check for
     * @return true if key exists in the map, false otherwise
     *
     * Time Complexity: O(1) average case, O(n) worst case
     * Space Complexity: O(1)
     */
    bool contains(const K &key) const;

    /**
     * @brief Applies a function to each key-value pair in the map.
     *
     * The function should accept two parameters: const K&, V&
     *
     * @tparam Func Callable type (e.g., function pointer, lambda, functor)
     * @param func The function to invoke on each key-value pair
     */
    template <typename Func>
    void forEach(Func func) {
        for (int i = 0; i < capacity_; i++) {
            Node* cur = buckets_[i];
            while (cur) {
                func(cur->key, cur->val);
                cur = cur->next;
            }
        }
    }

    /**
     * @brief Remove all entries from the map
     *
     * Removes and deallocates all nodes, but retains the bucket array.
     * Does not deallocate memory pointed to by stored pointers.
     *
     * Time Complexity: O(n) where n is the number of elements
     * Space Complexity: O(1)
     */
    void clear();

    /**
     * @brief Get the number of key-value pairs
     *
     * @return The current number of entries in the map
     *
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    int size() const { return size_; }

    /**
     * @brief Check if the map is empty
     *
     * @return true if map contains no entries, false otherwise
     *
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    bool empty() const { return size_ == 0; }

private:
    /**
     * @struct Node
     * @brief Internal node structure for linked list chains
     *
     * Each bucket in the hash table contains a linked list of nodes
     * for handling collisions via separate chaining.
     */
    struct Node {
        K key;          ///< The Key
        V val;          ///< The value
        Node *next;     ///< Pointer to next node in the chain

        /**
         * @brief Node constructor
         * @param k The key to store
         * @param v The value to store
         */
        Node(K k, const V &v) : key(std::move(k)), val(v), next(nullptr) {}
    };

    Node **buckets_;    ///< Array of bucket pointers (linked list heads)
    int capacity_;      ///< Current number of buckets
    int size_;          ///< Current number of key-value pairs
    float max_load_;    ///< Maximum load factor before rehashing

    /**
     * @brief Hash function for keys
     *
     * Uses std::hash to compute hash value for the given key.
     *
     * @param key The key to hash
     * @return Hash value for the key
     *
     * Time Complexity: O(1) for most types
     */
    static size_t hash(const K &key);

    /**
     * @brief Rehash the table with a new capacity
     *
     * Creates a new bucket array and redistributes all existing entries.
     * Called automatically when load factor exceeds threshold.
     *
     * @param newCap The new capacity (number of buckets)
     *
     * Time Complexity: O(n) where n is the number of elements
     * Space Complexity: O(m) where m is new capacity
     */
    void rehash(int newCap);

    /**
     * @brief Check and maintain load factor
     *
     * If current load factor exceeds max_load_, triggers rehashing
     * with double the current capacity.
     *
     * Time Complexity: O(1) if no rehash needed, O(n) if rehashing
     * Space Complexity: O(1) if no rehash needed, O(n) if rehashing
     */
    void ensureCapacity();
};

// ---------------- Implementation -------------------

template<typename K, typename V>
HashMap<K, V>::HashMap() : buckets_(nullptr), capacity_(16), size_(0), max_load_(0.75f) {
    buckets_ = new Node *[capacity_];
    for (int i = 0; i < capacity_; i++) buckets_[i] = nullptr;
}

template<typename K, typename V>
HashMap<K, V>::~HashMap() {
    clear();
    delete[] buckets_;
}

template<typename K, typename V>
size_t HashMap<K, V>::hash(const K &key) {
    return std::hash<K>{}(key);
}

template<typename K, typename V>
void HashMap<K, V>::rehash(const int newCap) {
    Node **newBuckets = new Node *[newCap];
    for (int i = 0; i < newCap; i++) newBuckets[i] = nullptr;

    for (int i = 0; i < capacity_; i++) {
        Node *cur = buckets_[i];
        while (cur) {
            Node *nxt = cur->next;
            const size_t h = hash(cur->key);
            int idx = static_cast<int>(h % newCap);
            cur->next = newBuckets[idx];
            newBuckets[idx] = cur;
            cur = nxt;
        }
    }

    delete[] buckets_;
    buckets_ = newBuckets;
    capacity_ = newCap;
}

template<typename K, typename V>
void HashMap<K, V>::ensureCapacity() {
    const float lf = (capacity_ == 0) ? 1.0f : static_cast<float>(size_) / static_cast<float>(capacity_);
    if (lf > max_load_) {
        int newCap = capacity_ * 2;
        if (newCap < 16) newCap = 16;
        rehash(newCap);
    }
}

template<typename K, typename V>
bool HashMap<K, V>::putNew(const K &key, const V &value) {
    const size_t h = hash(key);
    int idx = static_cast<int>(h % capacity_);
    Node *cur = buckets_[idx];
    while (cur) {
        if (cur->key == key) return false;
        cur = cur->next;
    }
    Node *nn = new Node(key, value);
    nn->next = buckets_[idx];
    buckets_[idx] = nn;
    size_ += 1;
    ensureCapacity();
    return true;
}

template<typename K, typename V>
V *HashMap<K, V>::get(const K &key) const {
    const size_t h = hash(key);
    int idx = static_cast<int>(h % capacity_);
    Node *cur = buckets_[idx];
    while (cur) {
        if (cur->key == key) return &cur->val;
        cur = cur->next;
    }
    return nullptr;
}

template<typename K, typename V>
bool HashMap<K, V>::erase(const K &key) {
    const size_t h = hash(key);
    int idx = static_cast<int>(h % capacity_);
    Node *cur = buckets_[idx];
    Node *prev = nullptr;
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else buckets_[idx] = cur->next;
            delete cur;
            size_ -= 1;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

template<typename K, typename V>
bool HashMap<K, V>::contains(const K &key) const {
    return get(key) != nullptr;
}

template<typename K, typename V>
void HashMap<K, V>::clear() {
    for (int i = 0; i < capacity_; i++) {
        Node *cur = buckets_[i];
        while (cur) {
            Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        buckets_[i] = nullptr;
    }
    size_ = 0;
}

#endif // ADS_PROJECT_HASH_MAP_H
