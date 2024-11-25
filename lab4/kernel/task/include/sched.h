#ifndef SCHED_H_
#define SCHED_H_

#include "task.h"
#include "utils.h"
#include "irq.h"

#define TASK_QUEUE_MAX_SIZE 10

typedef struct task_queue
{
    unsigned int queue[TASK_QUEUE_MAX_SIZE];
    unsigned int size;
}TASK_QUEUE;

extern TASK_QUEUE runq;

void task_schedule();
void task_timer_tick();
void task_queue_insert(TASK_QUEUE *tq, unsigned int id);
void task_queue_delete(TASK_QUEUE *tq, unsigned int id);

#endif