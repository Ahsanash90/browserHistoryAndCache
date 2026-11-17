#pragma once
template <typename T>
class Stack {
    struct Node { T data; Node* next; Node(const T& d, Node* n) : data(d), next(n) {} };
    Node* topNode;
    int count;
public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& v) { topNode = new Node(v, topNode); count++; }      // O(1)
    bool pop(T& out) {                                                       // O(1)
        if (!topNode) return false;
        Node* t = topNode;
        out = t->data;
        topNode = t->next;
        delete t;
        count--;
        return true;
    }
    bool empty() const { return topNode == nullptr; }
    int size() const { return count; }
    void clear() {
        while (topNode) { Node* t = topNode; topNode = t->next; delete t; }
        count = 0;
    }
};
#pragma once
