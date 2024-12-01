#ifndef TASK_H_
#define TASK_H_

#include "uart.h"
#include "define.h"
#include "sched.h"

#define TASK_POOL_NUM       10
#define TASK_INVALID_ID     TASK_POOL_NUM
#define INIT_TASK_ID        0
#define KERNEL_STACK_SIZE   4096
#define KERNEL_STACK_BASE   0x70000
#define USER_STACK_SIZE     4096
#define USER_STACK_BASE     0x20000
#define TASK_COUNTER_NUM    5

//
#define PF_KERNEL           0
#define PF_USER             1

//PSR
#define PSR_MODE_EL0t	0x00000000
#define PSR_MODE_EL1t	0x00000004
#define PSR_MODE_EL1h	0x00000005
#define PSR_MODE_EL2t	0x00000008
#define PSR_MODE_EL2h	0x00000009
#define PSR_MODE_EL3t	0x0000000c
#define PSR_MODE_EL3h	0x0000000d

enum task_state
{
    TASK_UNUSED,
    TASK_IDLE,
    TASK_RUNNING,
    TASK_ZOMBIE,
};

typedef struct pt_regs
{
	unsigned long regs[31];
	unsigned long lr;
	unsigned long pstate;
    unsigned long sp;
}PT_REGS;

#define PT_REGS_SIZE sizeof(PT_REGS)

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

typedef struct task_struct
{
    CPU_CONTEXT cpu_context;
    unsigned int state;
    int priority;
    int counter;
    int id;
    bool resched;
    bool preempt;
    unsigned int flag;
}TASK;

extern TASK task_pool[TASK_POOL_NUM];

typedef void(*task_callback)();

int task_create(unsigned int flag, void (*fn), unsigned long int arg);
void task_context_switch(TASK* next);
void task_pool_init();
TASK *task_get_current();
int task_user_exec(int id, void(*fn));
void task_resched();
int task_user_fork();
PT_REGS *task_get_pt_regs(int id);
void task_exit(int status);

extern void task_switch_to(TASK * pre, TASK *next);
extern void task_ret_from_fork();

#endif

