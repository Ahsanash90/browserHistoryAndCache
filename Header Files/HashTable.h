#pragma once
#include <string>
using namespace std;

template <typename V>
class HashTable
{
    struct Node
    {
        string key;
        V value;
        Node* next;

        Node(const string& k, Node* n)
            : key(k), value(), next(n) {
        }
    };

    Node** buckets;
    int cap, count;

    static unsigned long hashStr(const string& s) {
        unsigned long h = 5381;

        for (size_t i = 0; i < s.size(); i++)
            h = h * 33 + (unsigned char)s[i];

        return h;
    }

    void rehash(int newCap) {
        Node** nb = new Node * [newCap]();

        for (int i = 0; i < cap; i++) {
            Node* n = buckets[i];

            while (n) {
                Node* nx = n->next;
                unsigned long idx = hashStr(n->key) % newCap;

                n->next = nb[idx];
                nb[idx] = n;
                n = nx;
            }
        }

        delete[] buckets;
        buckets = nb;
        cap = newCap;
    }

public:
    HashTable(int c = 101)
        : buckets(new Node* [c]()), cap(c), count(0) {
    }

    ~HashTable() {
        for (int i = 0; i < cap; i++) {
            Node* n = buckets[i];

            while (n) {
                Node* nx = n->next;
                delete n;
                n = nx;
            }
        }

        delete[] buckets;
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    V* find(const string& key) const {
        for (Node* n = buckets[hashStr(key) % cap]; n; n = n->next)
            if (n->key == key)
                return &n->value;

        return nullptr;
    }

    V* getOrCreate(const string& key, bool* created = nullptr) {
        V* f = find(key);

        if (f) {
            if (created) *created = false;
            return f;
        }

        if (count + 1 > cap * 3 / 4)
            rehash(cap * 2 + 1);

        unsigned long idx = hashStr(key) % cap;

        buckets[idx] = new Node(key, buckets[idx]);
        count++;

        if (created) *created = true;

        return &buckets[idx]->value;
    }

    bool remove(const string& key) {
        unsigned long idx = hashStr(key) % cap;

        Node* prev = nullptr;
        Node* n = buckets[idx];

        while (n) {
            if (n->key == key) {
                if (prev)
                    prev->next = n->next;
                else
                    buckets[idx] = n->next;

                delete n;
                count--;

                return true;
            }

            prev = n;
            n = n->next;
        }

        return false;
    }

    int size() const {
        return count;
    }

    int capacity() const {
        return cap;
    }
};