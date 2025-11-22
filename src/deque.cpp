#include "deque.h"
#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int InitDeque(LinkDeque &D)
{
    D.front = (DequePtr)malloc(sizeof(DequeNode));
    if(!D.front) return ERROR;
    D.front->next = NULL;
    D.rear = D.front;
    return OK;
}
int DestroyDeque(LinkDeque &D)
{
    DequePtr p = D.front;
    while (p){
        DequePtr q = p->next;
        free(p);
        p = q;
    }
    D.front = NULL;
    D.rear = NULL;
    return OK;
}
int ClearDeque(LinkDeque &D)
{
    DequePtr p = D.front->next;
    while (p) {
        DequePtr q = p;
        p = p->next;
        free(q);
    }
    D.front->next = NULL;
    D.rear = D.front;
    return OK;
}
int DequeEmpty(LinkDeque &D)
{
    if(D.front->next == NULL) return TRUE;
    else return FALSE; 
}
int DequeLength(LinkDeque &D)
{
    int len = 0;
    DequePtr p = D.front->next;
    while(p){
        len++;
        p = p->next;
    }
    return len;
}
int GetHead(LinkDeque &D, ElemType &e)
{
    if(DequeEmpty(D)) return ERROR;
    e = D.front->next->data;
    return OK;
}
int GetTail(LinkDeque &D, ElemType &e)
{
    if(DequeEmpty(D)) return ERROR;
    e = D.rear->data;
    return OK;
}
int EnDeque(LinkDeque &D, ElemType e)
{
    DequePtr p = (DequePtr)malloc(sizeof(DequeNode));
    if(!p) return ERROR;
    p->data = e;
    p->next = NULL;
    D.rear->next = p;
    D.rear = p;
    return OK;
}
int DeDeque(LinkDeque &D, ElemType &e)
{
    if(DequeEmpty(D)) return ERROR;
    DequePtr p = D.front->next;
    e = p->data;
    D.front->next = p->next;
    if(D.rear == p) D.rear = D.front;
    free(p);
    return OK;
}
int DequeTraver(LinkDeque &D)
{
    DequePtr p = D.front->next;
    while(p){
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}