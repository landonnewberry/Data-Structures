#include <stdio.h>
#include <stdlib.h>
#include "darray.h"

typedef struct DArray {
    void** array;
    int capacity;
    int size;
    void (*display)(FILE *, void *);
} DArray;


extern DArray* newDArray(void (*display)(FILE *, void *))
{
    DArray* arr = (DArray*)malloc(sizeof(DArray));
    arr->capacity = 1;
    arr->size = 0;
    arr->array = (void**)malloc(sizeof(void*));
    arr->array[0] = NULL;
    arr->display = display;
    return arr;
}

extern void insertDArray(DArray* a, void* v)
{
    if (!v) v = NULL;
    int capacity = a->capacity;
    int size = a->size;
    int i;

    if (size == (capacity - 1))
    {
        // grow the array
        void** newArr = (void**)malloc(sizeof(void*) * (capacity * 2));
        
        for (i = 0; i < (capacity * 2); i++)
            newArr[i] = NULL;

        for (i = 0; i < capacity; i++)
            newArr[i] = a->array[i];

        a->capacity *= 2;

        // free old array
        free(a->array);
        a->array = NULL;

        // reassign
        a->array = newArr;

        a->array[size] = v;
    }
    else
    {
        a->array[size] = v;
    }
    a->size++;
}

extern void displayDArray(FILE* fp, DArray* a)
{
    void** arr = a->array;
    int size = a->size;
    int capacity = a->capacity;
    int i;

    // if array not empty
    if (size)
    {
        fprintf(fp, "[");

        for (i = 0; i < size; i++)
        {
            if (arr[i])
            {
                a->display(fp, arr[i]);
            }
            else
            {
                fprintf(fp, "NULL");
            }
            if (i != (size - 1))
                fprintf(fp, ",");
        }

        fprintf(fp, "][%d]", capacity - size);
    }

    else
    {
        fprintf(fp, "[][%d]", capacity - size);
    }
}

extern void* removeDArray(DArray* a)
{
    int i;
    int size = a->size;
    int capacity = a->capacity;

    void* value = NULL;

    // if array is not empty
    if (size)
    {
        // the element to be deleted and returned
        value = a->array[size - 1];

        // if only one element in array
        /*
        if (size == 1)
        {  
            free(a->array[0]);
            a->array[0] = NULL;
            a->capacity = 2;
        }
        */

        // need to shrink array
        if ((size - 1) < (capacity / 4))
        {
            // new array of half capacity
            void** newArr = (void**)malloc(sizeof(void*) * (capacity / 2));

            for (i = 0; i < (size - 1); i++)  // intentionally leave out last element
                newArr[i] = a->array[i];

            free(a->array);
            a->array = NULL;

            a->array = newArr;

            a->capacity /= 2;
        }

        // remove as usual
        else
        {
            free(a->array[size - 1]);
            a->array[size - 1] = NULL;
        }

        a->size--;
    }

    return value;
}

extern void* getDArray(DArray* a, int index)
{
    return a->array[index];
}

extern void setDArray(DArray* a, int index, void* value)
{
    if (!value) value = NULL;
    // onto the end of the array
    if (index == a->size)
    {
        insertDArray(a, value);
    }

    // replace current value
    else if (index < a->size)
    {
        a->array[index] = value;
    }
}

extern int sizeDArray(DArray* a)
{
    return a->size;
}