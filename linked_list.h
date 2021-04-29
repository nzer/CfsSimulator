#pragma once
#include "task.h"

typedef struct linked_node
{
    task_t data;
    struct linked_node *prev;
    struct linked_node *next;
} linked_node_t;

typedef struct linked_list
{
    struct linked_node *root;
    size_t size;
} linked_list_t;

void push(linked_list_t *list, task_t task);

void erase(linked_node_t *node);
