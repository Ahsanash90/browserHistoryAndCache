#pragma once
#include <iostream>
#include <string>
#include "Stack.h"
#include "UI.h"

struct Tab {
    int id;
    std::string url;               // current page ("" = blank tab)
    Stack<std::string> back, forward;
    Tab* next, * prev;
    Tab(int i) : id(i), next(this), prev(this) {}
};

class TabList {
    Tab* cur;        // currently active tab
    int count, nextId;
public:
    TabList() : cur(nullptr), count(0), nextId(1) {}
    ~TabList() { while (cur) closeCurrent(); }
    TabList(const TabList&) = delete;
    TabList& operator=(const TabList&) = delete;

    Tab* addTab(const std::string& url = "") {            // O(1), inserted after current
        Tab* t = new Tab(nextId++);
        t->url = url;
        if (!cur) { cur = t; }
        else {
            t->next = cur->next; t->prev = cur;
            cur->next->prev = t; cur->next = t;
            cur = t;
        }
        count++;
        return t;
    }
    bool closeCurrent() {                                  // O(1)
        if (!cur) return false;
        Tab* gone = cur;
        if (count == 1) cur = nullptr;
        else {
            gone->prev->next = gone->next;
            gone->next->prev = gone->prev;
            cur = gone->next;
        }
        delete gone;
        count--;
        return true;
    }
    void nextTab() { if (cur) cur = cur->next; }           // O(1)
    void prevTab() { if (cur) cur = cur->prev; }           // O(1)
    Tab* current() const { return cur; }
    int size() const { return count; }

    // The tab with the smallest id: gives tabs a stable display order while switching.
    Tab* firstTab() const {
        Tab* first = cur; Tab* t = cur;
        do { if (t->id < first->id) first = t; t = t->next; } while (t != cur);
        return first;
    }
    int position() const {                                 // 1-based position of the active tab
        if (!cur) return 0;
        int rank = 1;
        for (Tab* x = firstTab(); x != cur; x = x->next) rank++;
        return rank;
    }
    // Draws the tab strip as a table; the active tab is marked with ">".
    void print() const {
        if (!cur) { ui::info("No tabs open."); return; }
        Tab* first = firstTab();
        const int w[5] = { 3, 5, 36, 6, 6 };
        const std::string head[5] = { "", "Tab", "Current page", "Back", "Fwd" };
        ui::tableHead(head, w, 5);
        Tab* t = first;
        do {
            std::string row[5] = { t == cur ? ">" : "", std::to_string(t->id),
                                   t->url.empty() ? "(blank tab)" : t->url,
                                   std::to_string(t->back.size()), std::to_string(t->forward.size()) };
            ui::tableRow(row, w, 5, t == cur ? ui::C_GREEN : nullptr);
            t = t->next;
        } while (t != first);
    }
    template <typename F> void forEach(F f) const {
        if (!cur) return;
        Tab* t = cur;
        do { f(t); t = t->next; } while (t != cur);
    }
};
