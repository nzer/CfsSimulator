#include <stdio.h>
#include "rbt.h"

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
        fscanf(input, "%s %zu %zu %zu", task.name, &task.pri, &task.burst_time, &task.appearance_time);
        insert(&tree, task);
    }
    fclose(input);
    return 0;
}