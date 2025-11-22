#include "sequencelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

void SequenceList::InitList() {
    length = 0;
}

void SequenceList::Insert(int index, int value) {
    if (index < 0 || index > length || length >= 1000) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = length; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    ++length;
}

int SequenceList::ListLength() const {
    return length;
}

int SequenceList::GetElem(int index) const {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

int InitList(SqList *L)
{
    if (!L) return ERROR;
    L->length = 0;
    return OK;
}

// 判断顺序表是否为空
int ListEmpty(SqList L)
{
    return L.length == 0 ? TRUE : FALSE;
}

// 获取顺序表长度
int ListLength(SqList L)
{
    return L.length;
}

// 获取第i个元素
int GetElem(SqList L, int i, ElemType *e)
{
    if (i < 1 || i > L.length || !e) return ERROR;
    *e = L.data[i - 1];
    return OK;
}

// 定位元素位置
int LocateElem(SqList L, ElemType e)
{
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) {
            return i + 1;
        }
    }
    return 0; 
}

// 插入元素
int ListInsert(SqList *L, int i, ElemType e)
{
    if (!L || i < 1 || i > L->length + 1) return ERROR;
    if (L->length >= MAXSIZE) return ERROR;

    for (int j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];
    }
    
    L->data[i - 1] = e;
    L->length++;
    return OK;
}

// 删除元素
int ListDelete(SqList *L, int i, ElemType *e)
{
    if (!L || i < 1 || i > L->length) return ERROR;
    
    if (e) {
        *e = L->data[i - 1];
    }

    for (int j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j];
    }
    
    L->length--;
    return OK;
}

// 清空顺序表
int ClearList(SqList *L)
{
    if (!L) return ERROR;
    L->length = 0;
    return OK;
}

// 打印顺序表
void PrintList(SqList L)
{
    printf("[");
    for (int i = 0; i < L.length; i++) {
        printf("%d", L.data[i]);
        if (i < L.length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// 快速排序的分区函数
int Partition(SqList *L, int low, int high)
{
    if (!L || low < 0 || high >= L->length || low > high) return ERROR;
    
    ElemType pivot = L->data[low];
    int i = low, j = high;
    
    while (i < j) {
        while (i < j && L->data[j] >= pivot) {
            j--;
        }
        if (i < j) {
            L->data[i] = L->data[j];
            i++;
        }

        while (i < j && L->data[i] <= pivot) {
            i++;
        }
        if (i < j) {
            L->data[j] = L->data[i];
            j--;
        }
    }
    
    L->data[i] = pivot;
    return i;
}

// 快速排序
void QuickSort(SqList *L, int low, int high)
{
    if (!L || low < 0 || high >= L->length || low >= high) return;
    
    int pivotPos = Partition(L, low, high);
    if (pivotPos == ERROR) return;
    
    QuickSort(L, low, pivotPos - 1);
    QuickSort(L, pivotPos + 1, high);
}

// 快速选择：求第K小元素（K从1开始）
int QuickSelect(SqList *L, int low, int high, int k)
{
    if (!L || low < 0 || high >= L->length || low > high || k < 1) {
        return QUICKSELECT_ERROR;
    }
    
    if (low == high) {
        return L->data[low];
    }
    
    int pivotPos = Partition(L, low, high);
    if (pivotPos < 0) {
        return QUICKSELECT_ERROR;
    }

    int rank = pivotPos - low + 1;
    
    if (rank == k) {
        return L->data[pivotPos];
    } else if (rank > k) {
        return QuickSelect(L, low, pivotPos - 1, k);
    } else {
        return QuickSelect(L, pivotPos + 1, high, k - rank);
    }
}

// 求第K小元素
int FindKthSmallest(SqList *L, int k)
{
    if (!L || k < 1 || k > L->length) {
        return QUICKSELECT_ERROR;
    }
    SqList temp;
    InitList(&temp);
    for (int i = 0; i < L->length; i++) {
        temp.data[i] = L->data[i];
    }
    temp.length = L->length;
    
    return QuickSelect(&temp, 0, temp.length - 1, k);
}

