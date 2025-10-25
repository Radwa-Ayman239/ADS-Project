//
// Created by hamdy on 10/17/2025.
//

#include "hash_map.h"
#include <functional>

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
    const auto newBuckets = new Node *[newCap];
    for (int i = 0; i < newCap; i++) newBuckets[i] = nullptr;

    for (int i = 0; i < capacity_; i++) {
        Node *cur = buckets_[i];
        while (cur) {
            Node *nxt = cur->next;
            const unsigned long h = hash(cur->key);
            const int idx = static_cast<int>(h % newCap);
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
    const int idx = static_cast<int>(h % capacity_);
    const Node *cur = buckets_[idx];
    while (cur) {
        if (cur->key == key) return false;

        cur = cur->next;
    }

    const auto nn = new Node(key, value);
    nn->next = buckets_[idx];
    buckets_[idx] = nn;
    size_ += 1;
    ensureCapacity();
    return true;
}

template<typename K, typename V>
V *HashMap<K, V>::get(const K &key) const {
    const size_t h = hash(key);
    const int idx = static_cast<int>(h % capacity_);
    const Node *cur = buckets_[idx];
    while (cur) {
        if (cur->key == key) return &(cur->val);
        cur = cur->next;
    }

    return nullptr;
}

template<typename K, typename V>
bool HashMap<K, V>::erase(const K &key) {
    const size_t h = hash(key);
    const int idx = static_cast<int>(h % capacity_);
    Node *cur = buckets_[idx];
    Node *prev = nullptr;
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else buckets_[idx] = cur->next;
            delete cur;
            size_--;
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
        const Node *cur = buckets_[i];
        while (cur) {
            const Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }

        buckets_[i] = nullptr;
    }

    size_ = 0;
}
