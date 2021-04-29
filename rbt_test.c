#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rbt.h"

void invariant_check1(node_t *node)
{
    if (node != NULL)
    {
        if (node->leftChild != NULL)
        {
            if (node->isRed)
                assert(node->leftChild->isRed == 0);
            invariant_check1(node->leftChild);
        }
        if (node->rightChild != NULL)
        {
            if (node->isRed)
                assert(node->rightChild->isRed == 0);
            invariant_check1(node->rightChild);
        }
    }
}

void dump_dot2(node_t *node, FILE *o)
{
    if (node != NULL)
    {
        fprintf(o, "%zu [label=\"%zu_%c\"];\n", node->data.vruntime, node->data.vruntime, node->isRed ? 'R' : 'B');
        if (node->leftChild != NULL)
        {
            fprintf(o, "%zu -> %zu;\n", node->data.vruntime, node->leftChild->data.vruntime);
            dump_dot2(node->leftChild, o);
        }
        if (node->rightChild != NULL)
        {
            fprintf(o, "%zu -> %zu;\n", node->data.vruntime, node->rightChild->data.vruntime);
            dump_dot2(node->rightChild, o);
        }
    }
}

void dump_dot(tree_t *tree)
{

    FILE *o = fopen("dotdump.txt", "w");
    fprintf(o, "digraph sample {");
    dump_dot2(tree->root, o);
    fprintf(o, "}");
    fclose(o);
}

void test_pop()
{
    tree_t tree;
    tree.root = NULL;
    int min = RAND_MAX;
    for (size_t i = 0; i < 5000; i++)
    {
        task_t task;
        task.name = "task1";
        task.vruntime = rand();
        insert(&tree, task);
        if (min > task.vruntime)
            min = task.vruntime;
    }
    task_t t = pop_min(&tree);
    assert(min == t.vruntime);
}

void test_pop2()
{
    tree_t tree;
    tree.root = NULL;
    for (size_t i = 0; i < 5000; i++)
    {
        task_t task;
        task.name = "task1";
        task.vruntime = rand();
        insert(&tree, task);
    }
    while (tree.root != NULL)
    {
        static size_t count = 0;
        printf("deleted: %zu\n", count++);
        pop_min(&tree);
        invariant_check1(tree.root);
    }
}

void test_pop_dump()
{
    tree_t tree;
    tree.root = NULL;
    size_t counter = 1;
    size_t cap = (rand() + 1) % 50 + 15;
    for (size_t i = 0; i < cap; i++)
    {
        task_t task;
        task.name = "task";
        task.vruntime = counter++;
        insert(&tree, task);
    }
    for (size_t i = 0; i < cap / 2; i++)
    {
        pop_min(&tree);
    }
    for (size_t i = 0; i < cap / 3; i++)
    {
        task_t task;
        task.name = "task";
        task.vruntime = counter++;
        insert(&tree, task);
    }
    for (size_t i = 0; i < cap / 4; i++)
    {
        pop_min(&tree);
    }
    dump_dot(&tree);
}

void test_increasing()
{
    tree_t tree;
    tree.root = NULL;
    for (size_t i = 0; i < 5000; i++)
    {
        task_t task;
        task.name = "task1";
        task.vruntime = i;
        insert(&tree, task);
    }
}

int main()
{
    test_pop_dump();
    test_pop();
    test_pop2();
    test_increasing();
    printf("Success \n");
    return 0;
}
