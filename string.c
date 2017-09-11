#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "string.h"
#include "queue.h"

string* newString(char* x)
{
    string* p = malloc(sizeof(string));
    if (p == 0) printf("out of memory\n");


    int ch;
    int i;
    int valIndex = 0;
    char* valString = (char*)malloc(sizeof(char) * strlen(x));
    for (i = 0; i < (int)strlen(x); i++)
    {
        ch = (int)x[i];

        // it's capital
        if ((ch > 64) && (ch < 91))
        {
            ch += 32;
            valString[valIndex] = (char)ch;
            valIndex++;
        }
        // it's lower
        else if (((ch > 96) && (ch < 123)) || (ch == 32))
        {
            if (ch == 32)
            {
                if (valString[valIndex - 1] != (char)ch)
                {
                    valString[valIndex] = (char)ch;
                    valIndex++;
                }
            }
            else
            {
                valString[valIndex] = (char)ch;
                valIndex++;
            }
        }
        else if (isspace(ch) && ch != 32)
        {
            if (valString[valIndex - 1] != (char)32)
            {
                valString[valIndex] = (char)32;
                valIndex++;
            }
        }
    }

    if (!strcmp("", valString))
        return NULL;


    p->value = valString;
    return p;
}

char* getString(string* v)
{
    return v->value;
}

char* setString(string* v, char* x)
{
    char* old = v->value;
    v->value = x;
    return old;
}

void displayString(FILE* fp, void* v)
{
    fprintf(fp, "\"%s\"", getString(v));
}

int compareString(void* v, void* w)
{
    return strcmp(getString(v), getString(w));
}

void freeString(string* v)
{
    free(v);
}