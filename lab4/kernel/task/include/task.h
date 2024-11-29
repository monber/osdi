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
#define USER_STACK_SIZE     4096
#define USER_STACK_BASE     0x20000
#define TASK_COUNTER_NUM    5

//PSR
#define PSR_MODE_EL0t	0x00000000
#define PSR_MODE_EL1t	0x00000004
#define PSR_MODE_EL1h	0x00000005
#define PSR_MODE_EL2t	0x00000008
#define PSR_MODE_EL2h	0x00000009
#define PSR_MODE_EL3t	0x0000000c
#define PSR_MODE_EL3h	0x0000000d

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
}CPU_CONTEXT;

enum task_state
{
    TASK_UNUSED,
    TASK_IDLE,
    TASK_RUNNING,
};

typedef struct pt_regs {
	unsigned long regs[31];
	unsigned long lr;
	unsigned long pstate;
    unsigned long sp;
}PT_REGS;

#define PT_REGS_SIZE sizeof(PT_REGS)

typedef struct task_control_block
{
    CPU_CONTEXT cpu_context;
    unsigned long int kpc;
    unsigned int state;
    int priority;
    int counter;
    int id;
    bool resched;
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
void task_user_exec(int id, task_callback cb);
void task_resched();
int task_fork();
PT_REGS *task_get_pt_regs(int id);

extern void task_switch_to(TASK * pre, TASK *next);
extern void task_run(TASK *task);
extern void task_ret_from_fork();

#endif

