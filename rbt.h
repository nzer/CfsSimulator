#pragma once
#include "task.h"

typedef struct node
{
    task_t data;
    struct node* parent;
    struct node* leftChild;
    struct node* rightChild;
    int isRed;
} node_t;

typedef struct tree
{
    struct node* root;
} tree_t;

task_t pop_min(tree_t* tree);
void insert(tree_t *tree, task_t task);
