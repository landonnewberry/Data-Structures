#include <stdio.h>
#include <stdlib.h>
#include "vbst.h"
#include "queue.h"

// VBST Value "Object"
typedef struct vbstVal
{
    void* value;
    int count;
    void (*display)(FILE *, void *);
    int (*compare)(void *, void *);
} vbstVal;


// helper function declarations
static vbstVal* findVBSTNode(vbst *, void *);
static void displayVBSTVal(FILE *, void *);
static int compareVBSTVal(void *, void *);
static vbstVal* newVBSTVal(void (*)(FILE *, void *), int (*)(void *, void *), void *);
static vbstVal* findVBSTNode(vbst *, void *);


static void displayVBSTVal(FILE* fp, void* vbval)
{
    struct vbstVal* val = vbval;

    val->display(fp, (void*)val->value);

    if ((int)val->count != 1)
        fprintf(fp, "-%d", (int)val->count);
}

// takes 2 'vbstVal's
static int compareVBSTVal(void* val, void* vall)
{
    struct vbstVal* val1 = val;
    struct vbstVal* val2 = vall;

    return val2->compare(val1->value, val2->value);
}

static vbstVal* newVBSTVal(void (*d)(FILE *, void *), int (*c)(void *, void *), void *val)
{
    vbstVal* newVal = (vbstVal*)malloc(sizeof(vbstVal));
    newVal->count = 1;
    newVal->value = val;
    newVal->display = d;        // displayString
    newVal->compare = c;
    return newVal;
}

vbst* newVBST(void (*d)(FILE *, void *), int (*c)(void *, void *))
{
    vbst* newVBST = (vbst*)malloc(sizeof(vbst));
    newVBST->tree = newBST(displayVBSTVal, compareVBSTVal);
    newVBST->size = 0;
    newVBST->words = 0;
    newVBST->display = d;
    newVBST->compare = c;
    return newVBST;
}

void insertVBST(vbst* vbstree, void* value)
{
    if (value)
    {
        vbstVal* val = findVBSTNode(vbstree, value);
        if (val)
        {
            val->count++;
            vbstree->words++;
        }
        else
        {
            vbstVal* newVal = newVBSTVal(vbstree->display, vbstree->compare, value);
            insertBST(vbstree->tree, newVal);
            vbstree->words++;
        }
    }
}

static vbstVal* findVBSTNode(vbst* vbstree, void* value)
{
    bstNode* node = vbstree->tree->root;
    vbstVal* valNode = newVBSTVal(vbstree->display, vbstree->compare, value);
    while (node)
    {
        if (node->value)
        {
            if (compareVBSTVal(valNode, node->value) == 0)
                return node->value;
            else if (compareVBSTVal(valNode, node->value) < 0)
                node = node->left;
            else
                node = node->right;
        }
    }
    return 0;
}

void displayVBST(FILE* fp, vbst* tree)
{
    bst* t = tree->tree;
    displayBST(fp, t);
}

int findVBST(vbst* vbstree, void* value)
{
    vbstVal* val = findVBSTNode(vbstree, value);
    if (val)
        return val->count;
    return 0;
}

void deleteVBST(vbst* vbstree, void* value)
{
    bstNode* node = findBSTNode(vbstree->tree, newVBSTVal(vbstree->display, vbstree->compare, value));
    
    if (node)
    {
        vbstVal* val = node->value;
        if (val->count > 1)
        {
            val->count--;
            vbstree->words--;
            return;
        }
        else
        {
            bstNode* d = swapToLeafBSTNode(node);
            pruneBSTNode(vbstree->tree, d);
            vbstree->words--;
        }
    }
    else
    {
        fprintf(stderr, "Value ");
        vbstree->display(stderr, value);
        fprintf(stderr, " not found.\n");
    }
}

int sizeVBST(vbst* vbstree)
{
    return (int)vbstree->tree->size;
}

int wordsVBST(vbst* vbstree)
{
    return vbstree->words;
}

void statisticsVBST(vbst* vbstree, FILE* fp)
{
    fprintf(fp, "Words/Phrases: %d\n", wordsVBST(vbstree));
    statisticsBST(vbstree->tree, fp);
}