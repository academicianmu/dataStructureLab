#ifndef STACK_H
#define STACK_H

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int SElemType;
typedef double DSElemType;

typedef struct {
    SElemType *base;
    SElemType *top;
    int stacklength;
    int stacksize;
}SqStack;

typedef struct {
    DSElemType *base;
    DSElemType *top;
    int stacklength;
    int stacksize;
}DStack;

// 原有int栈函数
int InitStack(SqStack &stack);
int DestroyStack(SqStack &stack);
int ClearStack(SqStack &stack);
int StackEmpty(SqStack &stack);
int StackLength(SqStack &stack);
int GetTop(SqStack &stack, SElemType &e);
int Push(SqStack &stack, SElemType e);
int Pop(SqStack &stack, SElemType &e);
int StackTraver(SqStack &stack);

// 新增double栈函数
int InitDStack(DStack &stack);
int DestroyDStack(DStack &stack);
int ClearDStack(DStack &stack);
int DStackEmpty(DStack &stack);
int DStackLength(DStack &stack);
int GetDTop(DStack &stack, DSElemType &e);
int PushDStack(DStack &stack, DSElemType e);
int PopDStack(DStack &stack, DSElemType &e);
int DStackTraver(DStack &stack);

#endif