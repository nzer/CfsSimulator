#pragma once
#include "unistd.h"

typedef struct task
{
    char *name;
    size_t pri;
    size_t burst_time;
    size_t appearance_time;
    size_t vruntime;

    size_t exit_time;
    int response_time;
    size_t last_run_time;
    size_t total_wait_time;
    size_t preemptions;
} task_t;

task_t task_create()
{
    task_t task;
    task.name = (char *)malloc(sizeof(char) * 50);
    task.pri = 0;
    task.burst_time = 0;
    task.appearance_time = 0;
    task.vruntime = 0;
    task.exit_time = 0;
    task.response_time = -1;
    task.last_run_time = 0;
    task.total_wait_time = 0;
    task.preemptions = 0;
    return task;
}

void task_destroy(task_t task)
{
    free(task.name);
}
