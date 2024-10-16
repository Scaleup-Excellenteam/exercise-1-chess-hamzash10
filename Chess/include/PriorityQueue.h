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
    int operator()(const T& a, const T& b) const {
        return a.second - b.second;
    }
};

template<typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
    list<T> queue;
    Comparator comp;
public:
    void push(const T& value);
    T pull() const;
    const void clear();
    bool empty() const;

};

template<typename T, typename Comparator>
const void PriorityQueue<T, Comparator>::clear() {
    queue.clear();
}

template<typename T, typename Comparator>
bool PriorityQueue<T, Comparator>::empty() const {
    return queue.empty();
}


template<typename T, typename Comparator>
T PriorityQueue<T, Comparator>::pull() const {
    T value = queue.front();
    return value;
}

template<typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T &value) {
    // add the path only if it contains three moves overall
    auto it = queue.begin();
    while (it != queue.end()) {
        if(comp(*it,value) > 0)
            ++it;
        else
            break;
    }
    queue.insert(it, value);

}

#endif //CHESS_PRIORITYQUEUE_H
