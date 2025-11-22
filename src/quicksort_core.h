#ifndef QUICKSORT_CORE_H
#define QUICKSORT_CORE_H

#include <chrono>
#include "../include/sequencelist.h"

struct SortResult {
    double timeMs;
    int comparisons;
    int swaps;
};

class QuicksortComparator {
public:
    // 排序算法实现
    void basicQuickSort(SqList &arr, int left, int right);
    void mergeSort(SqList &arr, int left, int right);
    
    // 性能比较
    SortResult testQuickSort(const SqList &data);
    SortResult testMergeSort(const SqList &data);
    
    // 重置性能计数器
    void resetPerformanceCounters();

private:
    int comparisonCount = 0;
    int swapCount = 0;
    
    // 内部辅助函数
    int basicPartition(SqList &arr, int left, int right);
    void merge(SqList &arr, int left, int mid, int right);
};

#endif // QUICKSORT_CORE_H
