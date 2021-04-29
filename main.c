#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"

#define TIMER 10

int main(int argc, char const *argv[])
{
    tree_t tree;
    tree.root = NULL;

    // Input format: name pri burst appearance
    FILE* input = fopen("/home/alim/Projects/CfsSimulator/input.txt", "r");
    int input_size = 0;
    fscanf(input, "%d", &input_size);
    for (size_t i = 0; i < input_size; i++)
    {
        task_t task;
        task.name = (char*) malloc(sizeof(char) * 50);
        fscanf(input, "%s %zu %zu %zu", task.name, &task.pri, &task.burst_time, &task.appearance_time);
        insert(&tree, task);
    }
    fclose(input);
    while (tree.root != NULL)
    {
        pop_min(&tree);
    }
    
    return 0;
}
