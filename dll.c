#include <stdio.h>
#include <stdlib.h>
#include "dll.h"


dll* newDLL(void (*d)(FILE*, void*))
{
    dll* theDLL = (dll*)malloc(sizeof(dll));
    theDLL->head = NULL;
    theDLL->tail = theDLL->head;
    theDLL->size = 0;
    theDLL->display = d;
    return theDLL;
}

void insertDLL(dll* items, int index, void* value)
{
    
    if (items->size == 0)
    {
        dllnode* newNode = (dllnode*)malloc(sizeof(dllnode));
        newNode->value = value;
        newNode->next = NULL;
        newNode->prev = NULL;
        items->head = newNode;
        items->tail = newNode;
        items->size = (int)items->size + 1;
        return;
    }
    else if (index == 0)
    {
        dllnode* newNode = (dllnode*)malloc(sizeof(dllnode));
        newNode->value = value;
        newNode->next = items->head;
        items->head->prev = newNode;
        newNode->prev = NULL;
        items->head = newNode;
        items->size = (int)items->size + 1;
        return;
    }
    else if (items->size == index)
    {
        dllnode* newNode = (dllnode*)malloc(sizeof(dllnode));
        newNode->value = value;
        newNode->prev = items->tail;
        newNode->next = NULL;
        items->tail->next = newNode;
        items->tail = newNode;
        items->size = (int)items->size + 1;
        return;
    }
    else
    {
        // if index is closer to the beginning
        if (index < (items->size) / 2)
        {
            dllnode* newNode = (dllnode*)malloc(sizeof(dllnode));
            newNode->value = value;
            dllnode* n = items->head;
            dllnode* b = NULL;
            int i = 0;
            while(n)
            {
                if(i == index)
                {
                    b->next = newNode;
                    newNode->prev = b;
                    newNode->next = n;
                    n->prev = newNode;
                    items->size = items->size + 1;
                    return;
                }
                if(!n->next)
                {
                    items->tail->next = newNode;
                    newNode->next = NULL;
                    newNode->prev = items->tail;
                    items->tail = newNode;
                    items->size = items->size + 1;
                    return;
                }
                ++i;
                b = n;
                n = n->next;
            }
        }
        // if index is closer to the end
        else 
        {
            dllnode* newNode = (dllnode*)malloc(sizeof(newNode));
            newNode->value = value;
            dllnode* b = items->tail;
            int i = (int)items->size - 1;
            while(b)
            {
                if(index == items->size - 1)
                {
                    if (!b->prev)
                    {
                        newNode->next = b;
                        b->prev = newNode;
                        items->head = newNode;
                        items->tail = b;
                        items->size = (int)items->size + 1;
                        return;
                    }
                    else
                    {
                        b->prev->next = newNode;
                        newNode->prev = b->prev;
                        newNode->next = b;
                        b->prev = newNode;
                        items->size = (int)items->size + 1;
                        return;
                    }
                }
                if (index == i)
                {
                    b->prev->next = newNode;
                    newNode->prev = b->prev;
                    newNode->next = b;
                    b->prev = newNode;
                    items->size = (int)items->size + 1;
                    return;
                }

                b = b->prev;
                --i;
            }
        }
    }
}

void displayDLL(FILE* fp, dll* items)
{
    dllnode* p = items->head;
    fprintf(fp, "[");
    while(p)
    {
        if(p->next)
        {
            items->display(fp, p->value);
            fprintf(fp, ",");
        }
        else
        {
            items->display(fp, p->value);
        }
        p = p->next;
    }
    fprintf(fp, "]");
}

void* removeDLL(dll* items, int index)
{
    if(items->size == 1)
    {
        void* returnThis = items->head->value;
        items->head = NULL;
        items->tail = items->head;
        items->size = 0;
        return returnThis;
    }
    if(index == 0)
    {
        void* returnThis = items->head->value;
        items->head = items->head->next;
        items->head->prev = NULL;
        items->size = (int)items->size - 1;
        return returnThis;
    }
    if(index == (int)items->size - 1)
    {
        void* returnThis = items->tail->value;
        items->tail = items->tail->prev;
        items->tail->next = NULL;
        items->size = (int)items->size - 1;
        return returnThis;
    }

    // if index is closer to beginning
    if (index <= (items->size) / 2)
    {
        int i = 0;
        dllnode* n = items->head;
        dllnode* b = NULL;
        while(n)
        {
            if(index == i)
            {
                void* returnThis = n->value;
                b->next = n->next;
                n = n->next;
                n->prev = b;
                items->size = (int)items->size - 1;
                return returnThis;
            }
            b = n;
            n = n->next;
            ++i;
        }
    }
    // if index is closer to end 
    else
    {
        int i = items->size - 1;
        dllnode* n = items->tail;
        while(n)
        {
            if (index == i)
            {
                void* returnThis = n->value;
                n->prev->next = n->next;
                n->next->prev = n->prev;
                items->size = (int)items->size - 1;
                return returnThis;
            }
            n = n->prev;
            --i;
        }
    }

    
    void* returnThis = NULL;
    return returnThis;
}

void unionDLL(dll* recipient, dll* donor)
{
    // neither are null
    if (recipient->head && donor->head)
    {
        recipient->tail->next = donor->head;
        donor->head->prev = recipient->tail;
        recipient->tail = donor->tail;
        donor->head = NULL;
        donor->tail = NULL;
        recipient->size = (int)recipient->size + donor->size;
        donor->size = 0;
        return;
    }
    // donor is null
    else if (recipient->head)
    {
        return;
    }
    // recipient is null 
    else if (donor->head)
    {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
        donor->head = NULL;
        donor->tail = NULL;
        recipient->size = (int)donor->size;
        donor->size = 0;
        return;
    }
    // both are null
    else
    {
        return;
    }
}

void* getDLL(dll* items, int index)
{
    int i = 0;
    dllnode* p = items->head;
    while(p)
    {
        if(index == i)
        {
            return p->value;
        }
        ++i;
        p = p->next;
    }
    void* returnThis = NULL;
    return returnThis;
}

int sizeDLL(dll* items)
{
    return (int)items->size;
}