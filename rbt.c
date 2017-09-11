#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"
#include "queue.h"

// RBT Value "Object"
typedef struct rbtVal
{
    void* value;
    int count;
    void (*display)(FILE *, void *);
    int (*compare)(void *, void *);
    int color;      // 1 = red, 0 = black
} rbtVal;


// helper function declarations
static rbtVal* findRBTNode(rbt *, void *);
static void displayRBTVal(FILE *, void *);
static int compareRBTVal(void *, void *);
static rbtVal* newRBTVal(void (*)(FILE *, void *), int (*)(void *, void *), void*);
static rbtVal* findRBTNode(rbt *, void *);


static void insertionFixUp(bstNode*);


static int getParentColor(bstNode*);
static int getUncleColor(bstNode*);
static void setParentColor(bstNode*, int);
static void setGrandParentColor(bstNode*, int);
static void setUncleColor(bstNode*, int);
static void setColor(bstNode*, int);
static int isLeftChild(bstNode*);
static int isRightChild(bstNode*);
static bstNode* getParent(bstNode*);
static bstNode* getGrandParent(bstNode*);
static bstNode* getUncle(bstNode*);
static bstNode* rightChild(bstNode*);
static bstNode* leftChild(bstNode*);
static void rightRotate(bstNode*);
static void leftRotate(bstNode*);
static void rotateToParent(bstNode*);




static void displayRBTVal(FILE* fp, void* rbtval)
{
    struct rbtVal* val = rbtval;

    val->display(fp, (void*)val->value);

    if ((int)val->count != 1)
        fprintf(fp, "-%d", (int)val->count);

    if (val->color)
        fprintf(fp, "-R");
    else
        fprintf(fp, "-B");
}

static int compareRBTVal(void* val, void* vall)
{
    struct rbtVal* val1 = val;
    struct rbtVal* val2 = vall;
    
    return val2->compare(val1->value, val2->value);
}

static rbtVal* newRBTVal(void (*d)(FILE *, void *), int (*c)(void *, void *), void* val)
{
    rbtVal* newVal = (rbtVal*)malloc(sizeof(rbtVal));
    newVal->count = 1;
    newVal->value = val;
    newVal->display = d;
    newVal->compare = c;
    newVal->color = 1;
    return newVal;
}

rbt* newRBT(void (*d)(FILE *, void *), int (*c)(void *, void *))
{
    rbt* newRBT = (rbt*)malloc(sizeof(rbt));
    newRBT->tree = newBST(displayRBTVal, compareRBTVal);
    newRBT->words = 0;
    newRBT->display = d;
    newRBT->compare = c;
    return newRBT;
}

void insertRBT(rbt* rbtree, void* value)
{
    if (value)
    {
        rbtVal* val = findRBTNode(rbtree, value);
        if (val)
        {
            val->count++;
            rbtree->words++;
        }
        else
        {
            rbtVal* newVal = newRBTVal(rbtree->display, rbtree->compare, value);
            bstNode* node = insertBST(rbtree->tree, newVal);
            rbtree->words++;

            insertionFixUp(node);
        }
    }
}

static rbtVal* findRBTNode(rbt* rbtree, void* value)
{
    bstNode* node = rbtree->tree->root;
    rbtVal* valNode = newRBTVal(rbtree->display, rbtree->compare, value);
    while (node)
    {
        if (node->value)
        {
            if (compareRBTVal(valNode, node->value) == 0)
                return node->value;
            else if (compareRBTVal(valNode, node->value) < 0)
                node = node->left;
            else node = node->right;
        }
    }
    return 0;
}

void displayRBT(FILE* fp, rbt* tree)
{
    bst* t = tree->tree;
    displayBST(fp, t);
}

int findRBT(rbt* rbtree, void* value)
{
    rbtVal* val = findRBTNode(rbtree, value);
    if (val) return val->count;
    else return 0;
}

void deleteRBT(rbt* rbtree, void* value)
{
    (void)rbtree;
    (void)value;
}

int sizeRBT(rbt* rbtree)
{
    return (int)rbtree->tree->size;
}

int wordsRBT(rbt* rbtree)
{
    return rbtree->words;
}

void statisticsRBT(rbt* rbtree, FILE* fp)
{
    fprintf(fp, "Words/Phrases: %d\n", wordsRBT(rbtree));
    statisticsBST(rbtree->tree, fp);
}

static void insertionFixUp(bstNode* x)
{
    while (1)
    {
        // x is the root
        if (!getParent(x)) break;

        // parent is black
        if (!getParentColor(x)) break;

        // uncle is red recoloring case
        if (getUncleColor(x))
        {
            setParentColor(x, 0);
            setUncleColor(x, 0);
            setGrandParentColor(x, 1);
            x = getGrandParent(x);
        }
        // else uncle is black
        else
        {
            bstNode* parent = getParent(x);
            if ((isRightChild(x) && isLeftChild(getParent(x))) || (isLeftChild(x) && isRightChild(getParent(x))))
            {
                rotateToParent(x);
                bstNode* oldX = x;
                x = parent;
                parent = oldX;                
            }
            setGrandParentColor(x, 1);
            setParentColor(x, 0);
            rotateToParent(getParent(x));
            break;
        }
    }

    // root is always black
    while (getParent(x)) x = getParent(x);
    setColor(x, 0);
}




// COLOR FUNCTIONS
//
static bstNode* getParent(bstNode* node)
{
    if (node->parent != node) return node->parent;
    else return 0;
}

static bstNode* getGrandParent(bstNode* node)
{
    if (node->parent->parent != node->parent) return node->parent->parent;
    else return 0;
}

static bstNode* getUncle(bstNode* node)
{
    if (isLeftChild(getParent(node)))
    {
        bstNode* rc = rightChild(getGrandParent(node));
        if (rc)
        {
            return rightChild(getGrandParent(node));
        }
        else
        {
            return 0;
        }
    }
    else
    {   
        bstNode* lc = leftChild(getGrandParent(node));
        if (lc)
        {
            return leftChild(getGrandParent(node));
        }
        else 
        {
            return 0;
        }
    }
}

static void rotateToParent(bstNode* node)
{
    if (isLeftChild(node))
        rightRotate(node);   // change back
    else
        leftRotate(node);
}

static void rightRotate(bstNode* a)
{
    bstNode* aRight = a->right;   // is the NULL?
    bstNode* b = getParent(a);

    if (getParent(b))
    {
        a->parent = b->parent;

        if (isRightChild(b)) a->parent->right = a;
        else a->parent->left = a;
        if (aRight)
        {
            b->left = aRight;
            aRight->parent = b;
        }
        else
        {
            b->left = NULL;
        }
        b->parent = a;
        a->right = b;
    }
    else        // b is root
    {
        bstNode* newB = (bstNode*)malloc(sizeof(bstNode));
        newB->value = b->value;
        newB->right = b->right;

        if (b->right)
            b->right->parent = newB;
            
        newB->left = a->right;
        
        if (newB->left)
            newB->left->parent = newB;
        
        newB->parent = b;
        b->right = newB;
        b->value = a->value;
        b->left = a->left;
        b->left->parent = b;
    }
}

static void leftRotate(bstNode* a)
{
    bstNode* aLeft = a->left;   // is the NULL?
    bstNode* b = a->parent;

    if (getParent(b))
    {
        a->parent = b->parent;

        if (isLeftChild(b)) a->parent->left = a;
        else a->parent->right = a;
        if (aLeft)
        {
            b->right = aLeft;
            aLeft->parent = b;
        }
        else
        {
            b->right = NULL;
        }
        b->parent = a;
        a->left = b;
    }
    else        // b is root
    {
        bstNode* newB = (bstNode*)malloc(sizeof(bstNode));
        newB->value = b->value;
        newB->left = b->left;

        if (b->left)
            b->left->parent = newB;
            
        newB->right = a->left;
        
        if (newB->right)
            newB->right->parent = newB;
        
        newB->parent = b;
        b->left = newB;
        b->value = a->value;
        b->right = a->right;
        b->right->parent = b;
    }
}

static int isLeftChild(bstNode* node)
{
    bstNode* p = getParent(node);
    if (p)
    {
        if (p->left == node) return 1;
        else return 0;
    }
    else return 0;
}

static int isRightChild(bstNode* node)
{
    bstNode* p = getParent(node);
    if (p)
    {
        if (p->right == node) return 1;
        else return 0;
    }
    else return 0;
}

static bstNode* rightChild(bstNode* node)
{
    if (node->right) return node->right;
    else return 0;
}

static bstNode* leftChild(bstNode* node)
{
    if (node->left) return node->left;
    else return 0;
}

static int getParentColor(bstNode* node)
{
    bstNode* p = getParent(node);
    if (p)
    {
        rbtVal* val = p->value;
        if (val)
            return val->color;
        else return 0;
    }
    return 0;
}

static int getUncleColor(bstNode* node)
{
    bstNode* u = getUncle(node);
    if (u)
    {
        rbtVal* val = u->value;
        if (val)
            return val->color;
        else return 0;
    }
    return 0;
}

static void setColor(bstNode* node, int c)
{
    rbtVal* val = node->value;
    val->color = c;
}

static void setGrandParentColor(bstNode* node, int c)
{
    bstNode* gp = getGrandParent(node);
    if (gp)
    {
        rbtVal* val = gp->value;
        val->color = c;
    }
}

static void setParentColor(bstNode* node, int c)
{
    bstNode* p = getParent(node);
    if (p)
    {
        rbtVal* val = p->value;
        val->color = c;
    }
}

static void setUncleColor(bstNode* node, int c)
{
    bstNode* u = getUncle(node);
    if (u)
    {
        rbtVal* val = u->value;
        val->color = c;
    }
}