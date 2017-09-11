#include <stdio.h>

#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

typedef struct string 
{
    char* value;
} string;

extern string *newString(char*);
extern char* getString(string*);
extern char* setString(string*, char*);
extern void displayString(FILE*, void*);
extern int compareString(void*, void*);
extern void freeString(string*);

#endif