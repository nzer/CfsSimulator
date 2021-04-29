#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"

// all tasks must run at least once
#define SCHED_LATENCY 50
// least time task will run
#define MIN_GRANULARITY 5

int main(int argc, char const *argv[])
{
    tree_t tree = tree_create();
    size_t current_time = 0;
    size_t total_pri = 0;

    // Input format: name pri burst appearance
    FILE *input = fopen("/home/alim/Projects/CfsSimulator/input.txt", "r");
    size_t tasks_count = 0;
    size_t max_appereance_time = 0;
    fscanf(input, "%d", &tasks_count);
    for (size_t i = 0; i < tasks_count; i++)
    {
        task_t task = task_create();
        fscanf(input, "%s %zu %zu %zu", task.name, &task.pri, &task.burst_time, &task.appearance_time);
        insert(&tree, task);
        total_pri += task.pri;
        if (task.appearance_time > max_appereance_time)
        {
            max_appereance_time = task.appearance_time;
        }
    }
    fclose(input);

    // Runs until no more tasks left
    while (tree.root != NULL)
    {
        task_t curr_task = pop_min(&tree);
        // Task will for minimal of latency/number of tasks
        size_t runtime = SCHED_LATENCY / (tree.size + 1) * (float)curr_task.pri / (float)total_pri;
        // Too many tasks to run during SCHED_LATENCY. Also enforce minimal granularity
        if (SCHED_LATENCY < tree.size * MIN_GRANULARITY || runtime < MIN_GRANULARITY)
        {
            runtime = MIN_GRANULARITY;
        }
        // Prevent exceeding burst time
        if (runtime > curr_task.burst_time - curr_task.vruntime)
        {
            runtime = curr_task.burst_time - curr_task.vruntime;
        }

        if (curr_task.response_time == -1)
        {
            curr_task.response_time = current_time - curr_task.appearance_time;
        }
        size_t waiting_time = current_time - curr_task.last_run_time;
        curr_task.total_wait_time += waiting_time;
        curr_task.last_run_time = current_time;

        curr_task.vruntime += runtime;
        current_time += runtime;

        printf("==============================\n");
        printf("Turn statistics:\n");
        printf("Current time: %zu\n", current_time);
        printf("Task %s run for: %zu\n", curr_task.name, runtime);
        printf("Task %s response time: %i\n", curr_task.name, curr_task.response_time);
        printf("Task %s last wait time: %zu\n", curr_task.name, waiting_time);
        printf("Task %s total wait time: %zu\n", curr_task.name, curr_task.total_wait_time);
        if (curr_task.vruntime != curr_task.burst_time)
        {
            curr_task.preemptions++;
            insert(&tree, curr_task);
        }
        else
        {
            printf("Task %s total preemptions: %zu\n", curr_task.name, curr_task.preemptions);
            printf("Task %s total waiting time: %zu\n", curr_task.name, current_time - curr_task.appearance_time - curr_task.burst_time);
            printf("Task %s finished\n", curr_task.name);
            total_pri -= curr_task.pri;
            task_destroy(curr_task);
        }
    }

    return 0;
}
