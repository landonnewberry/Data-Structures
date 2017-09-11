#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "dll.h"

stack* newStack(void (*d)(FILE*, void*))
{
    stack* theStack = (stack*)malloc(sizeof(stack));
    theStack->stackDLL = newDLL(d);
    return theStack;
}

void push(stack* items, void* value)
{
    insertDLL(items->stackDLL, 0, value);
}

void* pop(stack* items)
{
    return removeDLL(items->stackDLL, 0);
}

void* peekStack(stack* items)
{
    return getDLL(items->stackDLL, 0);
}

int sizeStack(stack* items)
{
    return sizeDLL(items->stackDLL);
}

void displayStack(FILE* fp, stack* items)
{
    displayDLL(fp, items->stackDLL);
}