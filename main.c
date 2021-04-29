#include <stdio.h>
#include <stdlib.h>
#include "rbt.c"
#include "linked_list.c"

// all tasks must run at least once
#define SCHED_LATENCY 50
// least time task will run
#define MIN_GRANULARITY 5

int main(int argc, char const *argv[])
{
    rbtree_t tree = tree_create();
    linked_list_t list = list_create();

    double current_time = 0;
    size_t total_pri = 0;

    // Needed for averaging
    double wait_time_total = 0;
    double response_time_total = 0;

    // Input format: name pri burst appearance
    FILE *input = fopen("/home/alim/Projects/CfsSimulator/input.txt", "r");
    size_t tasks_count = 0;
    fscanf(input, "%zu", &tasks_count);
    for (size_t i = 0; i < tasks_count; i++)
    {
        task_t task = task_create();
        fscanf(input, "%s %zu %zu %zu", task.name, &task.pri, &task.burst_time, &task.appearance_time);
        push(&list, task);
    }
    fclose(input);

    // Runs until no more tasks left
    while (tree.root != NULL || list.root != NULL)
    {
        // Look for suitable tasks
        linked_node_t *lnode = list.root;
        while (lnode != NULL)
        {
            if (lnode->data.appearance_time <= current_time)
            {
                lnode->data.vruntime = current_time;
                total_pri += lnode->data.pri;
                insert(&tree, lnode->data);
                erase(&list, lnode);
                break;
            }
            lnode = lnode->next;
        }

        if (tree.root == NULL)
        {
            current_time++;
            continue;
        }

        task_t curr_task = pop_min(&tree);
        // Task will run for minimal of latency/number of tasks and adjust for priority
        double latency = SCHED_LATENCY;
        if (latency / (tree.size + 1) < MIN_GRANULARITY)
        {
            latency = MIN_GRANULARITY * (tree.size + 1);
        }
        double runtime = latency * (float)curr_task.pri / (float)total_pri;
        // Too many tasks to run during SCHED_LATENCY. Also enforce minimal granularity
        if (SCHED_LATENCY < (tree.size + 1) * MIN_GRANULARITY || runtime < MIN_GRANULARITY)
        {
            runtime = MIN_GRANULARITY;
        }
        // Prevent exceeding left burst time
        if (runtime > curr_task.burst_time - curr_task.runtime)
        {
            runtime = curr_task.burst_time - curr_task.runtime;
        }

        if (curr_task.response_time == -1) // Compute how long process waited for first run
        {
            curr_task.response_time = current_time - curr_task.appearance_time;
            response_time_total = curr_task.response_time;
        }
        double waiting_time = curr_task.response_time; // Tasks runs first time and already waited for response_time
        if (curr_task.last_run_time != 0)              // Otherwise waiting time is computed from last_run_time
        {
            waiting_time = current_time - curr_task.last_run_time;
        }
        curr_task.total_wait_time += waiting_time;
        curr_task.last_run_time = current_time + runtime;

        curr_task.vruntime += runtime * (float)curr_task.pri / (float)total_pri;
        curr_task.runtime += runtime;
        current_time += runtime;

        printf("==============================\n");
        printf("Turn statistics:\n");
        printf("Current time: %f\n", current_time);
        printf("    task name: %s\n", curr_task.name);
        printf("    run for: %f\n", runtime);
        printf("    competing tasks: %zu\n", tree.size);
        printf("    response time: %f\n", curr_task.response_time);
        printf("    last wait time: %f\n", waiting_time);
        printf("    total wait time: %f\n", curr_task.total_wait_time);
        if (curr_task.runtime != curr_task.burst_time)
        {
            curr_task.preemptions++;
            insert(&tree, curr_task);
        }
        else
        {
            printf("Task %s finished, total statistics:\n", curr_task.name);
            printf("    total preemptions: %zu\n", curr_task.preemptions);
            printf("    total waiting time (formula): %f\n", current_time - curr_task.appearance_time - curr_task.burst_time);
            wait_time_total += curr_task.total_wait_time;
            total_pri -= curr_task.pri;
            task_destroy(curr_task);
        }
    }
    printf("Simulation finished\n");
    printf("Average response time: %f\n", (double)response_time_total / tasks_count);
    printf("Average waiting time: %f\n", (double)wait_time_total / tasks_count);
    return 0;
}
