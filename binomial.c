#include <stdio.h>
#include <stdlib.h>
#include "binomial.h"
#include "darray.h"
#include "queue.h"


typedef struct BinomialNode {
    void* value;
    DArray* children;
    struct BinomialNode* parent;
    void (*display)(FILE *, void *);
} BinomialNode;

typedef struct Binomial {
    DArray* rootlist;
    int (*compare)(void *, void *);
    void (*update)(void *, BinomialNode *);
    BinomialNode* extreme;
    int size;
    void (*display)(FILE *, void *);
} Binomial;


/************ helper function declarations ***************/
// allocates new BinomialNode struct
BinomialNode* newBinomialNode(void (*display)(FILE *, void *), void* value);
// combines two subheaps with representatives x and y
static BinomialNode* combine(Binomial* b, BinomialNode* x, BinomialNode* y);
// gets the degree of a subheap
static int getDegree(BinomialNode* n);
// if a subheap exists at the given index, return it
static BinomialNode* getSubHeap(DArray* da, int index);
// consolidate the binomial heap with a new subheap
static void consolidate(Binomial* b, BinomialNode* n);
// bubble up a BinomialNode if its value deserves it
static BinomialNode* bubbleUp(Binomial* b, BinomialNode* n);
// take each subheap in the donor array and merge it into the heap b
static void merge(Binomial* b, DArray* donor);
// reset the extreme pointer of a Binomial heap
static BinomialNode* getExtreme(Binomial* b);

/************ display functions may need changing ************/
static void displayNewLine(FILE* fp, BinomialNode* n, int level);
static void displayBinomials(FILE* fp, BinomialNode* n, queue* q);






// not in pseudocode... definitely correct
BinomialNode* newBinomialNode(void (*display)(FILE *, void *), void* value)
{
    BinomialNode* bn = (BinomialNode*)malloc(sizeof(BinomialNode));
    bn->value = value;
    bn->children = newDArray(display);
    bn->parent = bn;
    bn->display = display;
    return bn;
}

// not in pseudocode... definitely correct
static BinomialNode* getExtreme(Binomial* b)
{
    int i;
    BinomialNode* newExtreme = NULL;
    int flag = 0;
    if (sizeDArray(b->rootlist))
    {
        newExtreme = getDArray(b->rootlist, 0);
        for (i = 0; i < sizeDArray(b->rootlist); i++)
        {
            if (getSubHeap(b->rootlist, i))
            {
                if (!flag)
                {
                    newExtreme = getSubHeap(b->rootlist, i);
                    flag = 1;
                }
                else
                {
                    BinomialNode* temp = getSubHeap(b->rootlist, i);
                    if (b->compare(temp->value, newExtreme->value) < 0)
                        newExtreme = temp;
                }
            }
        }
    }

    return newExtreme;
}

// not in pseudocode... definitely correct
static int getDegree(BinomialNode* n)
{
    return (sizeDArray(n->children));
}

// not in pseudocode... definitely correct
static BinomialNode* getSubHeap(DArray* da, int index)
{
    int size = sizeDArray(da);

    if (index > size) exit(0);
    if (index == size) return NULL;
    return getDArray(da, index);
}


// conforms perfectly to pseudocode
extern Binomial* newBinomial(void (*d)(FILE *, void *), int (*c)(void *, void *), void (*u)(void *, BinomialNode *))
{
    Binomial* b = (Binomial*)malloc(sizeof(Binomial));
    b->display = d;
    b->compare = c;
    b->update = u;
    b->rootlist = newDArray(d);
    b->extreme = NULL;
    b->size = 0;
    return b;
}

// conforms to pseudocode but I added a check to see if extreme should be updated... 
// I think Lusth forgot to do this
extern BinomialNode* insertBinomial(Binomial* b, void* value)
{
    // b is a binomial heap, v is the value to be inserted
    BinomialNode* n = newBinomialNode(b->display, value);

    consolidate(b, n);

    // fix extreme if necessary
    if (b->size != 0)
    {
        BinomialNode* extreme = b->extreme;
        if (b->compare(n->value, extreme->value) < 0)
            b->extreme = n;
    }
    else b->extreme = n;

    b->size++;
    

    return n;
}


// conforms perfectly to pseudocode
static BinomialNode* combine(Binomial* b, BinomialNode* x, BinomialNode* y)
{
    int i;
    if (b->compare(x->value, y->value) < 0)
    {
        i = getDegree(y);
        setDArray(x->children, i, y);
        y->parent = x;
        return x;
    }
    else
    {
        i = getDegree(x);
        setDArray(y->children, i, x);
        x->parent = y;
        return y;
    }
}

// conforms perfectly to pseudocode
static void consolidate(Binomial* b, BinomialNode* n)
{
    int degree = getDegree(n);

    while (getSubHeap(b->rootlist, degree))
    {
        BinomialNode* subheap = getSubHeap(b->rootlist, degree);
        n = combine(b, n, subheap);
        setDArray(b->rootlist, degree, NULL);
        degree++;
    }

    setDArray(b->rootlist, degree, n);
}




























// conforms perfectly to pseudocode
BinomialNode* bubbleUp(Binomial* b, BinomialNode* bn)
{
    if ((bn->parent == bn) || (b->compare(bn->value, bn->parent->value) > 0)) 
    	return bn;
    else
    {
        if (b->update != NULL)
        {
            b->update(bn->value, bn->parent);
            b->update(bn->parent->value, bn);
        }

        void* temp = bn->value;
        bn->value = bn->parent->value;
        bn->parent->value = temp;

        return bubbleUp(b, bn->parent);
    }
}

// conforms perfectly to pseudocode
static void merge(Binomial* b, DArray* donor)
{
    int i;
    int size = sizeDArray(donor);

    for (i = 0; i < size; i++)
    {
        if (getSubHeap(donor, i))
        {
            BinomialNode* n = getSubHeap(donor, i);
            n->parent = n;
            consolidate(b, n);
        }
    }
    // free(donor);    // this may not work
}

// conforms perfectly to pseudocode
extern void decreaseKeyBinomial(Binomial* b, BinomialNode* n, void* value)
{
    n->value = value;

    n = bubbleUp(b, n);
    
    // reset the extreme pointer to its new value
    b->extreme = getExtreme(b);
}


// conforms perfectly to pseudocode
extern void* extractBinomial(Binomial* b)
{
    int i;
    BinomialNode* y = b->extreme;
    // setDArray(b->rootlist, getDegree(y), NULL);

    for (i = 0; i < sizeDArray(b->rootlist); i++)
    {
        if (getSubHeap(b->rootlist, i))
        {
            BinomialNode* sh = getSubHeap(b->rootlist, i);
            if (y == sh)
            {
                setDArray(b->rootlist, i, NULL);
                break;
            }
        }
    }

    merge(b, y->children);
    b->size--;
    void* value = y->value;
    free(y);        // may not work 


    // find the new extreme node 
    b->extreme = getExtreme(b);

    return value;   // returns the value that was removed from heap
}


extern void deleteBinomial(Binomial* b, BinomialNode* n)
{
    decreaseKeyBinomial(b, n, NULL);
    extractBinomial(b);
}

extern int sizeBinomial(Binomial* b)
{
    return b->size;
}







/************************** display stuff **********************/
extern void displayBinomial(FILE* fp, Binomial* b)
{
    int i;
    for (i = 0; i < sizeDArray(b->rootlist); i++)
    {
        if (getSubHeap(b->rootlist, i))
        {
            BinomialNode* n = getSubHeap(b->rootlist, i);


            fprintf(fp, "0: ");
            displayBinomialNode(fp, n);

            // print newline if the node isnt an degree 0 subheap
            if (getDegree(n))
                fprintf(fp, "\n");

            queue* q = newQueue(b->display);

            if (getDegree(n))
                fprintf(fp, "1: ");

            displayBinomials(fp, n, q);

            fprintf(fp, "\n----");
            if (i != (sizeDArray(b->rootlist)))
                fprintf(fp, "\n");
        }
    }
}

static void displayNewLine(FILE* fp, BinomialNode* n, int level)
{
    if ((n->parent == n) && (sizeDArray(n->children)))
    {
        if (level != 0)
            fprintf(fp, "\n%d: ", level);
        return;
    }
    else
    {
        if (getDegree(n) == (getDegree(n->parent) - 1))
        {
            displayNewLine(fp, n->parent, level + 1);
        }
        else return;
    }
}

static void displayBinomials(FILE* fp, BinomialNode* n, queue* q)
{
    int c = getDegree(n);
    if (c)
    {
        int i;
        for (i = 0; i < c; i++)
        {
            BinomialNode* node = getDArray(n->children, i);

            displayBinomialNode(fp, node);

            // if not last node in subheap
            if (sizeQueue(q) && getDegree(node))
                displayNewLine(fp, node, 1);
            
            enqueue(q, node);
        }
    }
    if (sizeQueue(q))
    {
        displayBinomials(fp, dequeue(q), q);
    }
}

void displayBinomialNode(FILE* fp, BinomialNode* n)
{
    n->display(fp, n->value);
    fprintf(fp, "-%d", getDegree(n));
    if (n->parent != n)
    {
        fprintf(fp, "(");
        n->display(fp, n->parent->value);
        fprintf(fp, "-%d", getDegree(n->parent));
        fprintf(fp, ")");
    }
    fprintf(fp, " ");
}