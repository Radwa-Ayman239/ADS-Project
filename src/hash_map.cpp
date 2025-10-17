//
// Created by hamdy on 10/17/2025.
//

#include "hash_map.h"

HashMap::HashMap() : buckets_(nullptr), capacity_(16), size_(0), max_load_(0.75f) {
    buckets_ = new Node *[capacity_];
    for (int i = 0; i < capacity_; i++) buckets_[i] = nullptr;
}

HashMap::~HashMap() {
    clear();
    delete[] buckets_;
}

unsigned long HashMap::hash(const string &s) {
    const auto *str = reinterpret_cast<const unsigned char *>(s.c_str());
    unsigned long h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + static_cast<unsigned long>(c);

    return h;
}

void HashMap::rehash(const int newCap) {
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

void HashMap::ensureCapacity() {
    const float lf = (capacity_ == 0) ? 1.0f : static_cast<float>(size_) / static_cast<float>(capacity_);
    if (lf > max_load_) {
        int newCap = capacity_ * 2;
        if (newCap < 16) newCap = 16;
        rehash(newCap);
    }
}

bool HashMap::putNew(const string &key, void *value) {
    const unsigned long h = hash(key);
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

void *HashMap::get(const string &key) const {
    const unsigned long h = hash(key);
    const int idx = static_cast<int>(h % capacity_);
    const Node *cur = buckets_[idx];
    while (cur) {
        if (cur->key == key) return cur->val;
        cur = cur->next;
    }

    return nullptr;
}

bool HashMap::erase(const string &key) {
    const unsigned long h = hash(key);
    const int idx = static_cast<int>(h % capacity_);
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

bool HashMap::contains(const string &key) const {
    return get(key) != nullptr;
}

void HashMap::clear() {
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
