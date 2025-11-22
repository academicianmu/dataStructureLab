#ifndef EDGELIST_H
#define EDGELIST_H

// 避免重定义宏
#ifndef LISTINCREMENT
#define LISTINCREMENT 10
#endif

#ifndef LIST_INIT_SIZE
#define LIST_INIT_SIZE 100
#endif

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

// Edge结构体在graph.h中定义，必须先包含graph.h
#include "graph.h"

// Edge专用的顺序表
typedef struct {
    Edge *elem;
    int length;
    int listsize;
}EdgeList;

// Edge专用顺序表函数
int InitEdgeList(EdgeList *L);
int DestroyEdgeList(EdgeList *L);
int ClearEdgeList(EdgeList *L);
int EdgeListEmpty(EdgeList L);
int EdgeListLength(EdgeList L);
int GetEdgeElem(EdgeList L, int i, Edge *e);
int LocateEdgeElem(EdgeList L, Edge e);
int EdgeListInsert(EdgeList *L, int i, Edge e);
int EdgeListDelete(EdgeList *L, int i, Edge *e);
int EdgeListTraverse(EdgeList L);

#endif // EDGELIST_H
