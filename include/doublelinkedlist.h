#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// 避免重定义宏
#ifndef LISTINCREMENT
#define LISTINCREMENT 10
#endif

#define DOUBLIST_MAXSIZE 100
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int ElemType;

namespace DoubleList {
    // 双向链表
    typedef struct Node{
        ElemType data;
        struct Node *next;  // 指向下一个节点
        struct Node *prev;  // 指向前一个节点
    }Node, *DoubleLinkList;
}

int InitList(DoubleList::DoubleLinkList *L);
int ListInsert(DoubleList::DoubleLinkList L, int i, ElemType e);
int ListDelete(DoubleList::DoubleLinkList L, int i, ElemType *e);
int LocateElem(DoubleList::DoubleLinkList L, ElemType e);
int ListEmpty(DoubleList::DoubleLinkList L);
int ClearList(DoubleList::DoubleLinkList L);
int DestroyList(DoubleList::DoubleLinkList *L);
int ListLength(DoubleList::DoubleLinkList L);
int GetElem(DoubleList::DoubleLinkList L, int i, ElemType *e);

#endif