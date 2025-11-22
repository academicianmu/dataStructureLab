#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>
int InitList(SqList &L)
{
    L.elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if(!L.elem) return ERROR;
    L.length = 0;
    L.listsize = MAXSIZE;
    return OK;
}

int ListInsert(SqList &L, int i, ElemType e)
{
    if(i < 1 || i > L.length + 1) return ERROR;
    if(L.length >= L.listsize)
    {
        ElemType *newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
        if(!newbase) return ERROR;
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }
    ElemType *q = &(L.elem[i-1]);
    for(ElemType *p = &(L.elem[L.length-1]); p >= q; --p)
    {
        *(p+1) = *p;
    }
    *q = e;
    ++L.length;
    return OK;
}

int ListDelete(SqList &L, int i, ElemType *e)
{
    if(i < 1 || i > L.length) return ERROR;
    *e = L.elem[i-1];
    for(ElemType *p = &(L.elem[i-1]); p <= &(L.elem[L.length-1]); ++p)
    {
        *(p) = *(p+1);
    }
    --L.length;
    return OK;
}

int LocateElem(SqList &L, ElemType e)
{
    for(int i = 0; i < L.length; i++)
    {
        if(L.elem[i] == e) return i + 1;
    }
    return 0;
}

int ListEmpty(SqList &L)
{
    if(L.length == 0) return TRUE;
    else return FALSE;
}

int ClearList(SqList &L)
{
    L.length = 0;
    return OK;
}

int DestroyList(SqList &L)
{
    free(L.elem);
    L.elem = NULL;
    L.length = 0;
    L.listsize = 0;
    return OK;
}

int ListLength(SqList &L)
{
    return L.length;
}

int GetElem(SqList &L, int i, ElemType *e)
{
    if(i < 1 || i > L.length) return ERROR;
    *e = L.elem[i-1];
    return OK;
}