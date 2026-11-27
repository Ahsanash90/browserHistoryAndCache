#pragma once
template <typename T>
class MinHeap {
    T** a;
    int n, cap;
    void place(int i, T* e) { a[i] = e; e->heapIndex = i; }
    void up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (!(*a[i] < *a[p])) break;
            T* tmp = a[i]; place(i, a[p]); place(p, tmp);
            i = p;
        }
    }
    void down(int i) {
        while (true) {
            int l = 2 * i + 1, r = l + 1, s = i;
            if (l < n && *a[l] < *a[s]) s = l;
            if (r < n && *a[r] < *a[s]) s = r;
            if (s == i) break;
            T* tmp = a[i]; place(i, a[s]); place(s, tmp);
            i = s;
        }
    }
    void grow() {
        int nc = cap * 2;
        T** na = new T * [nc];
        for (int i = 0; i < n; i++) na[i] = a[i];
        delete[] a;
        a = na; cap = nc;
    }
public:
    MinHeap(int c = 16) : a(new T* [c < 2 ? 2 : c]), n(0), cap(c < 2 ? 2 : c) {}
    ~MinHeap() { delete[] a; }              // heap does NOT own the elements
    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    void push(T* e) { if (n == cap) grow(); place(n, e); n++; up(n - 1); }
    T* peek() const { return n ? a[0] : nullptr; }
    T* popMin() {
        if (!n) return nullptr;
        T* top = a[0];
        n--;
        if (n > 0) { place(0, a[n]); down(0); }
        return top;
    }
    void fix(T* e) { up(e->heapIndex); down(e->heapIndex); }   // after e's key changed
    T* at(int i) const { return a[i]; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
};
