#ifndef SEQUENCELIST_H
#define SEQUENCELIST_H

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 1000
#define MAXSIZE 20000
#define QUICKSELECT_ERROR -1
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int ElemType;

typedef struct {
    ElemType data[MAXSIZE];  // 数据数组
    int length;
    int listsize;
}SqList;

// 原有int顺序表函数
int InitList(SqList *L);
int DestroyList(SqList *L);
int ClearList(SqList *L);
int ListEmpty(SqList L);
int ListLength(SqList L);
int GetElem(SqList L, int i, ElemType *e);
int LocateElem(SqList L, ElemType e);
int ListInsert(SqList *L, int i, ElemType e);
int ListDelete(SqList *L, int i, ElemType *e);
int ListTraverse(SqList L);

// 快速排序相关函数
int Partition(SqList *L, int low, int high);
void QuickSort(SqList *L, int low, int high);
int QuickSelect(SqList *L, int low, int high, int k);
int FindKthSmallest(SqList *L, int k);

// 打印函数
void PrintList(SqList L);

class SequenceList {
public:
    void InitList();
    void Insert(int index, int value);
    int ListLength() const;
    int GetElem(int index) const;

private:
    int data[1000];
    int length;
};

#endif // SEQUENCELIST_H

