#include <stdlib.h>
#include <assert.h>
#include "rbt.h"

static rbnode_t *create_node()
{
    rbnode_t *node = malloc(sizeof(rbnode_t));
    node->parent = NULL;
    node->isRed = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

static void _leftRotation(rbtree_t *tree, rbnode_t *x)
{
    if (x->rightChild == NULL)
    {
        return;
    }
    rbnode_t *y = x->rightChild;
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

static void _rightRotation(rbtree_t *tree, rbnode_t *y)
{
    if (y->leftChild == NULL)
    {
        return;
    }
    rbnode_t *x = y->leftChild;
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

static void _fixup(rbtree_t *tree, rbnode_t *z)
{
    while (z != tree->root && z->parent->isRed)
    {
        if (z->parent == z->parent->parent->leftChild)
        {
            rbnode_t *y = z->parent->parent->rightChild;
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
            rbnode_t *y = z->parent->parent->leftChild;
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

static void _move(rbtree_t *tree, rbnode_t *u, rbnode_t *v)
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

static void _fixup2(rbtree_t *t, rbnode_t *x)
{
    while (x != t->root && !x->isRed)
    {
        if (x == x->parent->leftChild)
        {
            rbnode_t *node_w = x->parent->rightChild;
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
            rbnode_t *node_w = x->parent->leftChild;
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

static rbnode_t *_find_min(rbnode_t *x)
{
    while (x->leftChild != NULL)
    {
        x = x->leftChild;
    }
    return x;
}

static void _delete(rbtree_t *tree, rbnode_t *z)
{
    rbnode_t *x = NULL;
    rbnode_t *y = z;
    int yWasRed = y->isRed;
    if (z->leftChild == NULL)
    {
        x = z->rightChild;
        _move(tree, z, z->rightChild);
    }
    else if (z->rightChild == NULL)
    {
        x = z->leftChild;
        _move(tree, z, z->leftChild);
    }
    else
    {
        y = _find_min(z->rightChild);
        yWasRed = y->isRed;
        x = y->rightChild;
        if (y->parent == z)
            x->parent = y;
        else
        {
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
    tree->size--;
}

// Return and delete minimal element
task_t pop_min(rbtree_t *tree)
{
    rbnode_t *min = _find_min(tree->root);
    task_t ret = min->data;
    _delete(tree, min);
    free(min);
    return ret;
}

// I believe deletion could be further optimized because only leftmost or root node are removed
task_t pop_min2(rbtree_t *tree)
{
    assert(tree->root != NULL);
    rbnode_t *node = tree->root;
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

void insert(rbtree_t *tree, task_t task)
{
    rbnode_t *y = NULL;
    rbnode_t *x = tree->root;
    rbnode_t *z = create_node();
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
    tree->size++;
}
