#pragma once
#include "unistd.h"

typedef struct task
{
    char* name;
    size_t pri;
    size_t burst_time;
    size_t appearance_time;
    size_t vruntime;
} task_t;
