#ifndef TASK_H_
#define TASK_H_

#include "uart.h"
#include "define.h"
#include "sched.h"

#define TASK_POOL_NUM       10
#define TASK_INVALID_ID     TASK_POOL_NUM
#define TASK_IDLE_ID        0
#define KERNEL_STACK_SIZE   4096
#define KERNEL_STACK_BASE   0x30000
#define TASK_COUNTER_NUM    5

typedef struct cpu_context
{
    unsigned long int x19;
    unsigned long int x20;
    unsigned long int x21;
    unsigned long int x22;
    unsigned long int x23;
    unsigned long int x24;
    unsigned long int x25;
    unsigned long int x26;
    unsigned long int x27;
    unsigned long int x28;
    unsigned long int fp;
    unsigned long int lr;
    unsigned long int sp;
    unsigned long int pc;
}CPU_CONTEXT;

enum task_state
{
    TASK_UNUSED,
    TASK_IDLE,
    TASK_RUNNING,
};

typedef struct task_control_block
{
    CPU_CONTEXT cpu_context;
    unsigned int state;
    int priority;
    int counter;
    //task_callback cb;
}TASK;

extern TASK task_pool[TASK_POOL_NUM];

typedef void(*task_callback)();

int privilege_task_create(task_callback cb);
void task_context_switch(TASK* next);
void task_set_state(TASK *cur, int state);
void task_idle();
void task_pool_init();
TASK *task_get_current();

extern void task_switch_to(TASK * pre, TASK *next);
extern void task_run(TASK *task);

#endif
