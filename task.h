#pragma once
#include "unistd.h"

typedef struct task
{
    char *name;
    size_t pri;
    size_t burst_time;
    size_t appearance_time;
    double vruntime;

    // size_t exit_time;
    double response_time;
    double last_run_time;
    double total_wait_time;
    size_t preemptions;
    double runtime;
} task_t;

task_t task_create()
{
    task_t task;
    task.name = (char *)malloc(sizeof(char) * 50);
    task.pri = 0;
    task.burst_time = 0;
    task.appearance_time = 0;
    task.vruntime = 0;
    // task.exit_time = 0;
    task.response_time = -1;
    task.last_run_time = 0;
    task.total_wait_time = 0;
    task.preemptions = 0;
    task.runtime = 0;
    return task;
}

void task_destroy(task_t task)
{
    free(task.name);
}
