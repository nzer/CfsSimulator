#include <stdlib.h>
#include "linked_list.h"

static linked_node_t *create_lnode()
{
    linked_node_t *node = malloc(sizeof(linked_node_t));
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void push(linked_list_t *list, task_t task)
{
    if (list->root == NULL)
    {
        list->root = create_lnode();
        list->root->data = task;
    }
    else
    {
        linked_node_t *last_node = list->root;
        while (last_node->next != NULL)
        {
            last_node = last_node->next;
        }
        last_node->next = create_lnode();
        last_node->next->data = task;
        last_node->next->prev = last_node;
    }
}

void erase(linked_node_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
}
