#include <stdio.h>

#ifndef __DLL_INCLUDED__
#define __DLL_INCLUDED__

typedef struct dllnode {
    void* value;
    struct dllnode* next;
    struct dllnode* prev;
} dllnode;

typedef struct dll {
    dllnode* head;
    dllnode* tail;
    int size;
    void (*display)(FILE *, void *);
} dll;

extern dll* newDLL(void (*d)(FILE*, void*));
extern void insertDLL(dll* items, int index, void* value);
extern void* removeDLL(dll* items, int index);
extern void unionDLL(dll* recipient, dll* donor);
extern void* getDLL(dll* items, int index);
extern int sizeDLL(dll* items);
extern void displayDLL(FILE *, dll* items);

#endif