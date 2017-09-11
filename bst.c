#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

bst* newBST(void (*d)(FILE *,void *),int (*c)(void *,void *))
{
    bst* theBST = (bst*)malloc(sizeof(bst));
    theBST->display = d;
    theBST->compare = c;
    theBST->size = 0;
    theBST->root = NULL;
    return theBST;
}

bstNode* insertBST(bst* tree, void* value)
{
    bstNode* newNode = (bstNode*)malloc(sizeof(bstNode)); // node to be inserted
    newNode->value = value;
    newNode->parent = newNode;
    newNode->left = NULL;
    newNode->right = NULL;

    bstNode* x = tree->root;

    while (x != NULL)                                   // find where to insert newNode
    {
        newNode->parent = x;
        if (tree->compare(newNode->value, x->value) < 0)
            x = x->left;
        else
            x = x->right;
    }

    if (newNode->parent == newNode)                        // if the tree was an empty tree
        tree->root = newNode;

    else if (tree->compare(newNode->value, newNode->parent->value) < 0)
        newNode->parent->left = newNode;

    else
        newNode->parent->right = newNode;

    tree->size++;
    return newNode;
}

void displayBST(FILE* fp, bst* tree)
{
    queue* Q1 = newQueue(tree->display);
    queue* Q2 = newQueue(tree->display);

    if (tree->size != 0 && tree->root != NULL)
    {
        int level = 0;
        enqueue(Q1, tree->root);
        while (sizeQueue(Q1) != 0)
        {
            fprintf(fp, "%d: ", level);
            int sizeQ1 = sizeQueue(Q1);
            for (int i = 0; i < sizeQ1; i++)
            {
                bstNode* node = dequeue(Q1);
                if (node->left) enqueue(Q2, node->left);
                if (node->right) enqueue(Q2, node->right);
                
                if (!node->left && !node->right)
                    fprintf(fp, "=");
                

                tree->display(fp, node->value);

                fprintf(fp, "(");
                

                if (node->parent == node)
                    tree->display(fp, node->value);
                else
                    tree->display(fp, node->parent->value);

                fprintf(fp, ")-");

                if (node->parent != node)
                {
                    if (node->parent->left == node)
                        fprintf(fp, "l");
                    else
                        fprintf(fp, "r");
                }

                if (i != (sizeQ1 - 1))
                    fprintf(fp, " ");
            }
            fprintf(fp, "\n");
            queue* temp;
            temp = Q1;
            Q1 = Q2;
            Q2 = temp;
            level++;
        }
    }
    if (tree->size == 0)
    {
        fprintf(fp, "0:\n");
    }
}

int findBST(bst* tree, void* value)
{
    bstNode* node = tree->root;
    int level = 0;
    while (node)
    {
        if (tree->compare(value, node->value) == 0)
            return level;
        else if (tree->compare(value, node->value) < 0)
            node = node->left;
        else
            node = node->right;
        level++;
    }
    return 0;
}

bstNode* findBSTNode(bst* tree, void* value)
{
    bstNode* node = tree->root;
    while (node)
    {
        if (tree->compare(value, node->value) == 0)
            return node;
        else if (tree->compare(value, node->value) < 0)
            node = node->left;
        else
            node = node->right;
    }
    return 0;
}

bstNode* swapToLeafBSTNode(bstNode* node)
{
    void* sval = node->value;

    if (node->left != NULL)
    {
        bstNode* p = node->left;
        while (p->right)
            p = p->right;

        void* s = p->value;
        p->value = sval;
        node->value = s;
        return swapToLeafBSTNode(p);
    }
    else if (node->right != NULL)
    {
        bstNode* p = node->right;
        while (p->left)
            p = p->left;
        
        void* s = p->value;
        p->value = sval;
        node->value = s;
        return swapToLeafBSTNode(p);
    }
    else return node;
}

void pruneBSTNode(bst* tree, bstNode* node)
{
    if (node->right == NULL && node->left == NULL)
    {
        if (node->parent == node)
        {
            tree->root = NULL;
            tree->size = 0;
            return;
        }

        bstNode* parent = node->parent;

        if ((parent->right && !parent->left) || (parent->left && !parent->right))
        {
            if (parent->right)
            {
                parent->right = NULL;
                node->parent = NULL;
            }
            else
            {
                parent->left = NULL;
                node->parent = NULL;
            }
            tree->size--;
            return;
        }

        if (parent->left == node)
        {
            parent->left = NULL;       // left?
            node->parent = NULL;
            //free(node);
        }
        else
        {
            parent->right = NULL;        // right?
            node->parent = NULL;
            //free(node);
        }
    }
    else if (node->right && !node->left)
    {
        // node is root
        if (node->parent == node)
        {
            tree->root = node->right;
            tree->root->parent = tree->root;
        }
        else
        {
            // node is left child
            if (node->parent->left == node)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            // node is right child
            else
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
    }
    else if (node->left && !node->right)
    {
        if (node->parent == node)
        {
            tree->root = node->left;
            tree->root->parent = tree->root;
        }
        else
        {
            // node is left child
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            // node is right child
            else
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
    }
    tree->size--;
}

int sizeBST(bst *tree)
{
    return tree->size;
}

static int min(int a, int b)
{
    if (a > b) return b;
    else return a;
}

static int max(int a, int b)
{
    if (a > b)
        return a;
    else return b;
}

static int minDepth(bstNode *node)
{
    if (node == NULL)
        return 0;
 
    if (node->left == NULL || node->right == NULL)
       return 1;
 
    if (!node->left)
       return minDepth(node->right) + 1;
 
    if (!node->right)
       return minDepth(node->left) + 1;
 
    return min(minDepth(node->left), minDepth(node->right)) + 1;
}

static int maxDepth(bstNode *node)
{
    if (node == NULL)
        return 0;
    
    int rdepth = maxDepth(node->left);
    int ldepth = maxDepth(node->right);
    
    if (rdepth == 0)
        return 1+ldepth;
    else if (ldepth == 0)
        return 1+rdepth;
    else return (1 + max(rdepth, ldepth));
}

void statisticsBST(bst* tree, FILE* fp)
{
    fprintf(fp, "Nodes: %d\n", sizeBST(tree));
    fprintf(fp, "Minimum depth: %d\n", minDepth(tree->root));
    fprintf(fp, "Maximum depth: %d\n", maxDepth(tree->root));
}

