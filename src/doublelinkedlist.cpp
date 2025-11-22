#include "doublelinkedlist.h"
#include <stdio.h>
#include <stdlib.h>

// 初始化双向链表
int InitList(DoubleList::DoubleLinkList *L)
{
    *L = (DoubleList::DoubleLinkList)malloc(sizeof(DoubleList::Node));
    if(!*L) return ERROR;
    (*L)->next = NULL;
    (*L)->prev = NULL;
    return OK;
}
int ListInsert(DoubleList::DoubleLinkList L, int i, ElemType e)
{
    DoubleList::DoubleLinkList p = L;
    int j = 0;
    
    while(p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    
    if(!p || j > i - 1) return ERROR;
    
    DoubleList::DoubleLinkList s = (DoubleList::DoubleLinkList)malloc(sizeof(DoubleList::Node));
    if(!s) return ERROR;
    s->data = e;
    
    s->next = p->next;
    if(p->next) p->next->prev = s;
    p->next = s;
    s->prev = p;
    
    return OK;
}
int ListDelete(DoubleList::DoubleLinkList L, int i, ElemType *e)
{
    DoubleList::DoubleLinkList p = L;
    int j = 0;
    
    while(p && j < i)
    {
        p = p->next;
        j++;
    }
    
    if(!p || j > i) return ERROR;
    
    *e = p->data;
    
    // 双向链表删除操作
    if(p->prev) p->prev->next = p->next;
    if(p->next) p->next->prev = p->prev;
    
    free(p);
    return OK;
}
int LocateElem(DoubleList::DoubleLinkList L, ElemType e)
{
    DoubleList::DoubleLinkList p = L->next;
    int j = 1;
    while(p && p->data != e)
    {
        p = p->next;
        j++;
    }
    
    if(p) return j;
    else return 0;
}
int ListEmpty(DoubleList::DoubleLinkList L)
{
    if(L->next == NULL) return TRUE;
    else return FALSE;
}
int ClearList(DoubleList::DoubleLinkList L)
{
    DoubleList::DoubleLinkList p = L->next;
    while(p)
    {
        DoubleList::DoubleLinkList q = p;
        p = p->next;
        free(q);
    }
    L->next = NULL;
    L->prev = NULL;
    return OK;
}
int DestroyList(DoubleList::DoubleLinkList *L)
{
    DoubleList::DoubleLinkList p = (*L)->next;
    while(p)
    {
        DoubleList::DoubleLinkList q = p;
        p = p->next;
        free(q);
    }
    free(*L);
    *L = NULL;
    return OK;
}
int ListLength(DoubleList::DoubleLinkList L)
{
    DoubleList::DoubleLinkList p = L->next;
    int j = 0;
    while(p)
    {
        p = p->next;
        j++;
    }
    return j;
}
int GetElem(DoubleList::DoubleLinkList L, int i, ElemType *e)
{
    DoubleList::DoubleLinkList p = L->next;
    int j = 1;
    while(p && j < i)
    {
        p = p->next;
        j++;
    }
    
    if(!p || j > i) return ERROR;
    *e = p->data;
    return OK;
}