#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#ifndef MAXSIZE
#define MAXSIZE 100
#endif

#define LISTINCREMENT 10
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int ElemType;

namespace SingleList {
    typedef struct Node{
        ElemType data;
        struct Node *next;
    }Node, *LinkList;
}

int InitList(SingleList::LinkList *L);
int ListInsert(SingleList::LinkList L, int i, ElemType e);
int ListDelete(SingleList::LinkList L, int i, ElemType *e);
int LocateElem(SingleList::LinkList L, ElemType e);
int ListEmpty(SingleList::LinkList L);
int ClearList(SingleList::LinkList L);
int DestroyList(SingleList::LinkList *L);
int ListLength(SingleList::LinkList L);
int GetElem(SingleList::LinkList L, int i, ElemType *e);

template <typename T>
class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void insert(int index, T value);
    T get(int index) const; // 添加 get 方法
    int size() const;       // 添加 size 方法

private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    int length; // 记录链表长度
};

#endif // LINKEDLIST_H