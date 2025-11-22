// 线性表
#ifndef MYLIST_H
#define MYLIST_H

typedef int ElemType;

#define MAXSIZE 100
#define LISTINCREMENT 10
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef struct{
    ElemType *elem;
    int length;
    int listsize;
}SqList;

int InitList(SqList &L);
int ListInsert(SqList &L, int i, ElemType e);
int ListDelete(SqList &L, int i, ElemType *e);
int LocateElem(SqList &L, ElemType e);
int ListEmpty(SqList &L);
int ClearList(SqList &L);
int DestroyList(SqList &L);
int ListLength(SqList &L);
int GetElem(SqList &L, int i, ElemType *e);

#endif

