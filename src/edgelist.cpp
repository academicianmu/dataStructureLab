#include "edgelist.h"
#include <stdio.h>
#include <stdlib.h>

// Edge专用顺序表实现
int InitEdgeList(EdgeList *L)
{
    L->elem = (Edge*)malloc(LIST_INIT_SIZE * sizeof(Edge));
    if(!L->elem) return ERROR;
    L->length = 0;
    L->listsize = LIST_INIT_SIZE;
    return OK;
}
int DestroyEdgeList(EdgeList *L)
{
    free(L->elem);
    L->elem = NULL;
    L->length = 0;
    L->listsize = 0;
    return OK;
}
int ClearEdgeList(EdgeList *L)
{
    L->length = 0;
    return OK;
}
int EdgeListEmpty(EdgeList L)
{
    if(L.length == 0)return TRUE;
    else return FALSE;
}
int EdgeListLength(EdgeList L)
{
    return L.length;
}
int GetEdgeElem(EdgeList L, int i, Edge *e)
{
    if(i < 1 || i > L.length)return ERROR;
    *e = L.elem[i-1];
    return OK;
}
int LocateEdgeElem(EdgeList L, Edge e)
{
    for(int i = 0; i < L.length; i++)
        if(L.elem[i].start == e.start && L.elem[i].end == e.end && L.elem[i].weight == e.weight) return i+1;
    return 0;
}
int EdgeListInsert(EdgeList *L, int i, Edge e)
{
    if(i < 1 || i > L->length+1) return ERROR;
    if(L->length >= L->listsize)
    {
        Edge *newbase = (Edge*)realloc(L->elem, (L->listsize+LISTINCREMENT)*sizeof(Edge));
        if(!newbase) return ERROR;
        L->elem = newbase;
        L->listsize += LISTINCREMENT;
    }
    for(int j = L->length; j >= i; j--)
        L->elem[j] = L->elem[j-1];
    L->elem[i-1] = e;
    L->length++;
    return OK;
}
int EdgeListDelete(EdgeList *L, int i, Edge *e)
{
    if(i < 1 || i > L->length) return ERROR;
    *e = L->elem[i-1];
    for(int j = i; j < L->length; j++)
        L->elem[j-1] = L->elem[j];
    L->length--;
    return OK;
}
int EdgeListTraverse(EdgeList L)
{
    if(L.length == 0) return ERROR;
    for(int i = 0; i < L.length; i++)
        printf("(%d,%d,%d) ", L.elem[i].start, L.elem[i].end, L.elem[i].weight);
    printf("\n");
    return OK;
}
