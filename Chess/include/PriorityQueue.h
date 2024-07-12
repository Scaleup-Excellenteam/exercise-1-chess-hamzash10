//
// Created by 2022 on 7/11/2024.
//

#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H

#include <list>
#include "Piece.h"
#include <algorithm>
using namespace std;


template<typename T>
struct MyComparator {
    int operator()(const pair<vector<Location>, int>& a, const pair<vector<Location>, int>& b) const;
};


template<typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
    list<T> queue;
    Comparator comp;
public:
    void push(const T& value);
    T pull();
};


template<typename T>
int MyComparator<T>::operator()(const pair<vector<Location>, int> &a, const pair<vector<Location>, int> &b) const  {
    return a.second - b.second;
}

template<typename T, typename Comparator>
T PriorityQueue<T, Comparator>::pull() {
    T value = queue.front();
    queue.pop_front();
    return value;
}

template<typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T &value) {
    auto it = queue.begin();
    while (it != queue.end() && comp(value, *it)) {
        ++it;
    }
    queue.insert(it, value);
}

#endif //CHESS_PRIORITYQUEUE_H
