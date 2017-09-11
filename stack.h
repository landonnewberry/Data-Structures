#include <stdio.h>
#include "dll.h"

#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__


typedef struct stack {
    dll* stackDLL;
} stack;


extern stack* newStack(void (*d)(FILE*, void*));
extern void push(stack* items, void* value);
extern void* pop(stack* items);
extern void* peekStack(stack* items);
extern int sizeStack(stack* items);
extern void displayStack(FILE* fp, stack* items);


#endif