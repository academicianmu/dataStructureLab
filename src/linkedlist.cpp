#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), length(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void LinkedList<T>::insert(int index, T value) {
    if (index < 0 || index > length) {
        throw std::out_of_range("Index out of range");
    }

    Node* newNode = new Node{value, nullptr};
    if (index == 0) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    ++length;
}

template <typename T>
T LinkedList<T>::get(int index) const {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index out of range");
    }

    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
int LinkedList<T>::size() const {
    return length;
}

int InitList(SingleList::LinkList *L)
{
    *L = (SingleList::LinkList)malloc(sizeof(SingleList::Node));
    if(!*L) return ERROR;
    (*L)->next = NULL;
    return OK;
}

// 在第i个位置之前插入元素e
int ListInsert(SingleList::LinkList L, int i, ElemType e)
{
    SingleList::LinkList p = L;
    int j = 0;
    while(p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if(!p || j > i - 1) return ERROR;
    
    SingleList::LinkList s = (SingleList::LinkList)malloc(sizeof(SingleList::Node));
    if(!s) return ERROR;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

// 删除第i个位置的元素
int ListDelete(SingleList::LinkList L, int i, ElemType *e)
{
    SingleList::LinkList p = L;
    int j = 0;
    while(p && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if(!p || j > i - 1 || !p->next) return ERROR;
    
    SingleList::LinkList q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    return OK;
}

int LocateElem(SingleList::LinkList L, ElemType e)
{
    SingleList::LinkList p = L->next;
    int j = 1;
    while(p && p->data != e)
    {
        p = p->next;
        j++;
    }
    if(!p) return 0;
    else return j;
}

int ListEmpty(SingleList::LinkList L)
{
    if(L->next == NULL) return TRUE;
    else return FALSE;
}

int ClearList(SingleList::LinkList L)
{
    SingleList::LinkList p = L->next;
    while(p){
        SingleList::LinkList q = p;
        p = p->next;
        free(q);
    }
    L->next = NULL;
    return OK;
}

int DestroyList(SingleList::LinkList *L)
{
    SingleList::LinkList p = *L;
    while(p){
        SingleList::LinkList q = p;
        p = p->next;
        free(q);
    }
    *L = NULL;
    return OK;
}

int ListLength(SingleList::LinkList L)
{
    SingleList::LinkList p = L->next;
    int j = 0;
    while(p){
        p = p->next;
        j++;
    }
    return j;
}

int GetElem(SingleList::LinkList L, int i, ElemType *e)
{
    SingleList::LinkList p = L->next;
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

// 显式实例化模板
template class LinkedList<int>;