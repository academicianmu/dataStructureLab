#ifndef JOSEPHUS_CORE_H
#define JOSEPHUS_CORE_H

#include <vector>
#include "../include/sequencelist.h"
#include "../include/doublelinkedlist.h"
#include "../include/deque.h"

class JosephusSolver {
public:
    // 三种不同的约瑟夫环实现
    std::vector<int> josephusWithSequenceList(int n, int m);
    std::vector<int> josephusWithDoublyLinkedList(int n, int m);
    std::vector<int> josephusWithDeque(int n, int m);
};

#endif // JOSEPHUS_CORE_H
