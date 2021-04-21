#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rbt.h"

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

void test_increasing() {
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
    test_pop();
    test_increasing();
    printf("Success \n");
    return 0;
}
