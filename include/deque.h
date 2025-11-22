// 队列
#ifndef MYDEQUE_H
#define MYDEQUE_H
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int ElemType;
typedef struct DequeNode{
    ElemType data;
    struct DequeNode *next;
}DequeNode, *DequePtr;

typedef struct{
    DequePtr front;
    DequePtr rear;
}LinkDeque;

int InitDeque(LinkDeque &D);
int DestroyDeque(LinkDeque &D);
int ClearDeque(LinkDeque &D);
int DequeEmpty(LinkDeque &D);
int DequeLength(LinkDeque &D);
int GetHead(LinkDeque &D, ElemType &e);
int GetTail(LinkDeque &D, ElemType &e);
int EnDeque(LinkDeque &D, ElemType e);
int DeDeque(LinkDeque &D, ElemType &e);
int DequeTraver(LinkDeque &D);

#endif