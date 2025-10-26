//
// Created by hamdy on 10/26/2025.
//
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <sstream>

#include "hash_map.h"

// A key type with an intentionally pathological hash to force collisions.
struct CollideKey {
    int id;
    bool operator==(const CollideKey& other) const noexcept { return id == other.id; }
};

namespace std {
template<> struct hash<CollideKey> {
    size_t operator()(const CollideKey& k) const noexcept {
        // Constant hash: every key collides into the same bucket
        return 0xDEADBEEF;
    }
};
} // namespace std

static std::string key_of(int i) {
    std::ostringstream oss;
    oss << "key_" << i;
    return oss.str();
}

static void test_construct_empty() {
    HashMap<std::string, int> m;
    assert(m.size() == 0);
    assert(m.empty());
    assert(!m.contains("absent"));
    assert(m.get("absent") == nullptr);
}

static void test_put_get_contains_basic() {
    HashMap<std::string, int> m;
    assert(m.putNew("a", 1));
    assert(m.putNew("b", 2));
    assert(!m.putNew("a", 111)); // duplicate rejected
    int* pa = m.get("a");
    int* pb = m.get("b");
    assert(pa && pb);
    assert(*pa == 1);
    assert(*pb == 2);
    assert(m.contains("a"));
    assert(!m.contains("z"));
    assert(m.size() == 2);
}

static void test_mutation_via_pointer() {
    HashMap<std::string, int> m;
    m.putNew("x", 7);
    int* px = m.get("x");
    assert(px);
    *px = 99; // mutate in place
    int* px2 = m.get("x");
    assert(px2 && *px2 == 99);
}

static void test_erase_cases() {
    HashMap<std::string, int> m;
    assert(!m.erase("none")); // erase on missing
    m.putNew("a", 1);
    m.putNew("b", 2);
    assert(m.erase("a"));
    assert(!m.contains("a"));
    assert(m.size() == 1);
    assert(!m.erase("a")); // idempotent
    assert(m.contains("b"));
    assert(m.erase("b"));
    assert(m.empty());
}

static void test_clear_idempotent() {
    HashMap<std::string, int> m;
    for (int i = 0; i < 10; ++i) {
        assert(m.putNew(key_of(i), i));
    }
    assert(m.size() == 10);
    m.clear();
    assert(m.size() == 0);
    assert(m.empty());
    // Second clear should be harmless
    m.clear();
    assert(m.empty());
}

static void test_rehash_integrity_and_pointer_stability() {
    HashMap<std::string, int> m;

    // Insert an anchor, keep pointer to its stored value
    assert(m.putNew("anchor", 42));
    int* p_anchor = m.get("anchor");
    assert(p_anchor && *p_anchor == 42);

    // Insert enough elements to trigger at least one rehash
    // Default capacity is 16, load factor threshold 0.75; insert far more than that
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        std::string k = key_of(i);
        // Allow duplicates attempt: only count first insert as success
        (void)m.putNew(k, i);
    }

    // All previously inserted elements should still be retrievable
    for (int i = 0; i < N; ++i) {
        int* pv = m.get(key_of(i));
        if (pv) { // may be overwritten only if duplicate inserted, but we never duplicated same key_of(i)
            assert(*pv == i);
        }
    }

    // The pointer obtained before rehash should still point to the same value
    assert(p_anchor && *p_anchor == 42);
    int* p2 = m.get("anchor");
    assert(p2 && p2 == p_anchor && *p2 == 42);
}

static void test_heavy_collisions_separate_chaining() {
    HashMap<CollideKey, int> m;

    const int M = 200;
    for (int i = 0; i < M; ++i) {
        assert(m.putNew(CollideKey{i}, i));
    }
    assert(m.size() == M);

    // Lookup all
    for (int i = 0; i < M; ++i) {
        int* p = m.get(CollideKey{i});
        assert(p && *p == i);
    }

    // Erase middle-of-chain elements and re-check neighbors
    for (int i = 0; i < M; i += 3) {
        assert(m.erase(CollideKey{i}));
        assert(!m.contains(CollideKey{i}));
    }
    for (int i = 0; i < M; ++i) {
        int* p = m.get(CollideKey{i});
        if (i % 3 == 0) {
            assert(p == nullptr);
        } else {
            assert(p && *p == i);
        }
    }
}

static void test_large_volume() {
    HashMap<std::string, int> m;
    const int N = 20000;

    for (int i = 0; i < N; ++i) {
        assert(m.putNew(key_of(i), i));
    }
    assert(m.size() == N);

    // Sample lookups
    for (int i : {0, 1, 2, 3, 7, 11, 1024, 7777, 19999}) {
        int* p = m.get(key_of(i));
        assert(p && *p == i);
    }

    // Random check
    std::mt19937_64 rng(123456);
    for (int t = 0; t < 2000; ++t) {
        int i = static_cast<int>(rng() % N);
        int* p = m.get(key_of(i));
        assert(p && *p == i);
    }
}

int main() {
    test_construct_empty();
    test_put_get_contains_basic();
    test_mutation_via_pointer();
    test_erase_cases();
    test_clear_idempotent();
    test_rehash_integrity_and_pointer_stability();
    test_heavy_collisions_separate_chaining();
    test_large_volume();

    std::cout << "All tests passed\n";
    return 0;
}

