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

void _move(tree_t *tree, node_t *u, node_t *v)
{
    if (u->parent == NULL)
    {
        tree->root = v;
    }
    else if (u == u->parent->leftChild)
    {
        u->parent->leftChild = v;
    }
    else
    {
        u->parent->rightChild = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}

void _fixup2(tree_t *t, node_t *x)
{
    while (x != t->root && !x->isRed)
    {
        if (x == x->parent->leftChild)
        {
            node_t *node_w = x->parent->rightChild;
            if (node_w->isRed)
            {
                node_w->isRed = 0;
                x->parent->isRed = 1;
                _leftRotation(t, x->parent);
                node_w = x->parent->rightChild;
            }
            else if (!node_w->leftChild->isRed && !node_w->rightChild->isRed)
            {
                node_w->isRed = 1;
                x = x->parent;
            }
            else if (!node_w->rightChild->isRed)
            {
                node_w->leftChild->isRed = 0;
                node_w->isRed = 1;
                _rightRotation(t, node_w);
                node_w = x->parent->rightChild;
            }
            else
            {
                node_w->isRed = x->parent->isRed;
                x->parent->isRed = 0;
                node_w->rightChild->isRed = 0;
                _leftRotation(t, x->parent);
                x = t->root;
            }
        }
        else if (x == x->parent->rightChild)
        {
            node_t *node_w = x->parent->leftChild;
            if (node_w->isRed)
            {
                node_w->isRed = 0;
                x->parent->isRed = 1;
                _leftRotation(t, x->parent);
                node_w = x->parent->rightChild;
            }
            else if (!node_w->leftChild->isRed && !node_w->rightChild->isRed)
            {
                node_w->isRed = 1;
                x = x->parent;
            }
            else if (!node_w->rightChild->isRed)
            {
                node_w->leftChild->isRed = 0;
                node_w->isRed = 1;
                _rightRotation(t, node_w);
                node_w = x->parent->rightChild;
            }
            else
            {
                node_w->isRed = x->parent->isRed;
                x->parent->isRed = 0;
                node_w->rightChild->isRed = 0;
                _leftRotation(t, x->parent);
                x = t->root;
            }
        }
    }
    x->isRed = 0;
}

node_t *_find_min(node_t *x)
{
    while (x->leftChild != NULL)
    {
        x = x->leftChild;
    }
    return x;
}

void _delete(tree_t *tree, node_t *z)
{
    node_t * x = NULL;
    node_t * y = z;
    int yWasRed = y->isRed;
    if (z->leftChild == NULL) {
        x = z->rightChild;
        _move(tree, z, z->rightChild);
    }
    else if (z->rightChild == NULL) {
        x = z->leftChild;
        _move(tree, z, z->leftChild);
    } else {
        y = _find_min(z->rightChild);
        yWasRed = y->isRed;
        x = y->rightChild;
        if (y->parent == z)
            x->parent = y;
        else {
            _move(tree, y, y->rightChild);
            y->rightChild = z->rightChild;
            y->rightChild->parent = y;
        }
        _move(tree, z, y);
        y->leftChild = z->leftChild;
        y->leftChild->parent = y;
        y->isRed = z->isRed;
    }
    if (!yWasRed && x != NULL)
        _fixup2(tree, x);
}

// Return and delete minimal element
task_t pop_min(tree_t *tree)
{
    node_t *min = _find_min(tree->root);
    task_t ret = min->data;
    _delete(tree, min);
    free(min);
    return ret;
}

task_t pop_min2(tree_t *tree)
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
        assert(tree->root == node);
        if (node->rightChild != NULL)
        {
            tree->root = node->rightChild;
        }
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

    task_t ret = node->data;
    free(node);
    return ret;
}
