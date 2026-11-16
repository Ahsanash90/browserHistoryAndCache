#pragma once
template <typename T>
class Queue {
    struct Node { T data; Node* next; Node(const T& d) : data(d), next(nullptr) {} };
    Node* head, * tail;
    int count;
public:
    Queue() : head(nullptr), tail(nullptr), count(0) {}
    ~Queue() { clear(); }
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& v) {                                               // O(1)
        Node* n = new Node(v);
        if (tail) tail->next = n; else head = n;
        tail = n;
        count++;
    }
    bool dequeue(T& out) {                                                   // O(1)
        if (!head) return false;
        Node* t = head;
        out = t->data;
        head = t->next;
        if (!head) tail = nullptr;
        delete t;
        count--;
        return true;
    }
    bool empty() const { return head == nullptr; }
    int size() const { return count; }
    void clear() {
        while (head) { Node* t = head; head = t->next; delete t; }
        tail = nullptr;
        count = 0;
    }
};
