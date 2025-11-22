#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// int栈实现
int InitStack(SqStack &stack)
{
    stack.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if(!stack.base) return ERROR;
    stack.top = stack.base;
    stack.stacksize = STACK_INIT_SIZE;
    stack.stacklength = 0;
    return OK;
}
int DestroyStack(SqStack &stack)
{
    free(stack.base);
    stack.base = stack.top = NULL;
    stack.stacksize = 0;
    stack.stacklength = 0;
    return OK;
}
int ClearStack(SqStack &stack)
{
    stack.top = stack.base;
    stack.stacklength = 0;
    return OK;
}
int StackEmpty(SqStack &stack)
{
    if(stack.stacklength == 0)return TRUE;
    else return FALSE;
}
int StackLength(SqStack &stack)
{
    return stack.stacklength;
}
int GetTop(SqStack &stack, SElemType &e)
{
    if(stack.top == stack.base)return ERROR;
    e = *(stack.top - 1);
    return OK;
}
int Push(SqStack &stack, SElemType e)
{
    if(stack.top - stack.base >= stack.stacksize){
        stack.base = (SElemType*)realloc(stack.base,(stack.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if(!stack.base)return ERROR;
        stack.top = stack.base + stack.stacksize;
        stack.stacksize += STACKINCREMENT;
    }
    *stack.top++ = e;
    stack.stacklength++;
    return OK;
}
int Pop(SqStack &stack, SElemType &e)
{
    if(stack.top == stack.base)return ERROR;
    e = *--stack.top;
    stack.stacklength--;
    return OK;
}
int StackTraver(SqStack &stack)
{
    if(stack.stacklength == 0)return ERROR;
    SElemType *p = stack.base;
    for(int i = 0; i < stack.stacklength; i++)
    {
        printf("%d ", *p++);
    }
    printf("\n");
    return OK;
}

// double栈实现
int InitDStack(DStack &stack)
{
    stack.base = (DSElemType*)malloc(STACK_INIT_SIZE * sizeof(DSElemType));
    if(!stack.base) return ERROR;
    stack.top = stack.base;
    stack.stacksize = STACK_INIT_SIZE;
    stack.stacklength = 0;
    return OK;
}
int DestroyDStack(DStack &stack)
{
    free(stack.base);
    stack.base = stack.top = NULL;
    stack.stacksize = 0;
    stack.stacklength = 0;
    return OK;
}
int ClearDStack(DStack &stack)
{
    stack.top = stack.base;
    stack.stacklength = 0;
    return OK;
}
int DStackEmpty(DStack &stack)
{
    if(stack.stacklength == 0)return TRUE;
    else return FALSE;
}
int DStackLength(DStack &stack)
{
    return stack.stacklength;
}
int GetDTop(DStack &stack, DSElemType &e)
{
    if(stack.top == stack.base)return ERROR;
    e = *(stack.top - 1);
    return OK;
}
int PushDStack(DStack &stack, DSElemType e)
{
    if(stack.top - stack.base >= stack.stacksize){
        stack.base = (DSElemType*)realloc(stack.base,(stack.stacksize + STACKINCREMENT) * sizeof(DSElemType));
        if(!stack.base)return ERROR;
        stack.top = stack.base + stack.stacksize;
        stack.stacksize += STACKINCREMENT;
    }
    *stack.top++ = e;
    stack.stacklength++;
    return OK;
}
int PopDStack(DStack &stack, DSElemType &e)
{
    if(stack.top == stack.base)return ERROR;
    e = *--stack.top;
    stack.stacklength--;
    return OK;
}
int DStackTraver(DStack &stack)
{
    if(stack.stacklength == 0)return ERROR;
    DSElemType *p = stack.base;
    for(int i = 0; i < stack.stacklength; i++)
    {
        printf("%f ", *p++);
    }
    printf("\n");
    return OK;
}