//
// Created by 2022 on 7/11/2024.
//

#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H

#include <list>
using namespace std;

struct MyComparator {
    int operator()(int a, int b) const;
};


template<typename T, typename Comparator>
class PriorityQueue {
    list<T> queue;
    Comparator comp;
public:
    void push(const T& value);
    T poll();
};


#include "../src/PriorityQueue.cpp"

#endif //CHESS_PRIORITYQUEUE_H
