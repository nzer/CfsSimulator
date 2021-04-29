#pragma once
#include "task.h"

typedef struct node
{
    task_t data;
    struct node *parent;
    struct node *leftChild;
    struct node *rightChild;
    int isRed;
} rbnode_t;

typedef struct tree
{
    struct node *root;
    size_t size;
} rbtree_t;

task_t pop_min(rbtree_t *tree);
void insert(rbtree_t *tree, task_t task);

rbtree_t tree_create()
{
    rbtree_t tree;
    tree.root = NULL;
    tree.size = 0;
    return tree;
}
