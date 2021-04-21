#include <stdlib.h>
#include <assert.h>
#include "rbt.h"

static node_t *create_node()
{
    node_t *node = malloc(sizeof(node_t));
    node->parent = NULL;
    node->isRed = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

static void _leftRotation(tree_t *tree, node_t *x)
{
    if (x->rightChild == NULL)
    {
        return;
    }
    node_t *y = x->rightChild;
    x->rightChild = y->leftChild;
    if (y->leftChild != NULL)
    {
        y->leftChild->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL)
    {
        tree->root = y;
    }
    else if (x == x->parent->leftChild)
    {
        x->parent->leftChild = y;
    }
    else
    {
        x->parent->rightChild = y;
    }
    y->leftChild = x;
    x->parent = y;
}

static void _rightRotation(tree_t *tree, node_t *y)
{
    if (y->leftChild == NULL)
    {
        return;
    }
    node_t *x = y->leftChild;
    y->leftChild = x->rightChild;
    if (x->rightChild != NULL)
    {
        x->rightChild->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL)
    {
        tree->root = x;
    }
    else if (y == y->parent->rightChild)
    {
        y->parent->rightChild = x;
    }
    else
    {
        y->parent->leftChild = x;
    }
    x->rightChild = y;
    y->parent = x;
}

static void _fixup(tree_t *tree, node_t *z)
{
    while (z != tree->root && z->parent->isRed)
    {
        if (z->parent == z->parent->parent->leftChild)
        {
            node_t *y = z->parent->parent->rightChild;
            if (y != NULL && y->isRed)
            {
                z->parent->isRed = 0;
                y->isRed = 0;
                z->parent->parent->isRed = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->rightChild)
                {
                    z = z->parent;
                    _leftRotation(tree, z);
                }
                z->parent->isRed = 0;
                z->parent->parent->isRed = 1;
                _rightRotation(tree, z->parent->parent);
            }
        }
        else
        {
            node_t *y = z->parent->parent->leftChild;
            if (y != NULL && y->isRed)
            {
                z->parent->isRed = 0;
                y->isRed = 0;
                z->parent->parent->isRed = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->leftChild)
                {
                    z = z->parent;
                    _rightRotation(tree, z);
                }
                z->parent->isRed = 0;
                z->parent->parent->isRed = 1;
                _leftRotation(tree, z->parent->parent);
            }
        }
    }
    tree->root->isRed = 0;
}

void insert(tree_t *tree, task_t task)
{
    node_t *y = NULL;
    node_t *x = tree->root;
    node_t *z = create_node();
    z->data = task;
    while (x != NULL)
    {
        y = x;
        if (task.vruntime < x->data.vruntime)
            x = x->leftChild;
        else
            x = x->rightChild;
    }
    z->parent = y;
    if (y == NULL)
        tree->root = z;
    else if (z->data.vruntime < y->data.vruntime)
        y->leftChild = z;
    else
        y->rightChild = z;
    z->leftChild = NULL;
    z->rightChild = NULL;
    z->isRed = 1;
    _fixup(tree, z);
}

task_t pop_min(tree_t *tree)
{
    assert(tree->root != NULL);
    node_t *node = tree->root;
    while (node->leftChild != NULL)
    {
        node = node->leftChild;
    }
    // Two cases for deletion
    // 1) This node is leaf
    // 2) This node has right child
    // 3) This node is root
    if (node->parent == NULL)
    {
        tree->root = node->rightChild;
    }
    else if (node->rightChild == NULL)
    {
        node->parent->leftChild = NULL;
    }
    else
    {
        node->parent->leftChild = node->rightChild;
        node->parent->leftChild->parent = node->parent;
    }
    // Color of moved node might be wrong
    _fixup(tree, node->parent);
    task_t ret = node->data;
    free(node);
    return ret;
}
