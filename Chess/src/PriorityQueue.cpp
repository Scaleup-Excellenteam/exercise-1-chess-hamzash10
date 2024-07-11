//
// Created by 2022 on 7/11/2024.
//

#include "../include/PriorityQueue.h"

int MyComparator::operator()(int a, int b) const {
    return a-b;
}
