#include "task.h"

TASK task_pool[TASK_POOL_NUM];
//char kstack_pool[TASK_POOL_NUM][KERNEL_STACK_SIZE];

void task_pool_init()
{
    //kernel_main
    task_pool[0].state = TASK_RUNNING;
    task_pool[0].priority = 0;
    task_pool[0].id = TASK_IDLE_ID;
    task_pool[0].counter = TASK_COUNTER_NUM;
    task_queue_insert(&runq, TASK_IDLE_ID);
    unsigned long int idle_task_adr = (unsigned long int)(&task_pool[0]);
    asm volatile("msr tpidr_el1, %0" : "=r" (idle_task_adr));
    //other
    for(int i = 1; i < TASK_POOL_NUM; i++)
    {
        task_pool[i].state = TASK_UNUSED;
    }
}

static int task_get_id()
{
    int id = 1;
    while(id < TASK_POOL_NUM)
    {
        if(task_pool[id].state == TASK_UNUSED)
        {
            task_pool[id].state = TASK_IDLE;
            break;
        }
        id++;
    }
    return id;
}

int privilege_task_create(task_callback cb)
{
    int id = task_get_id();
    if(id == TASK_INVALID_ID)
    {
        uart_printf("Warn: can't create privilege task\n\r");
        return TASK_INVALID_ID;
    }
    task_pool[id].cpu_context.lr = (unsigned long int)task_ret_from_fork;
    task_pool[id].cpu_context.fp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    task_pool[id].cpu_context.sp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    task_pool[id].kpc = (unsigned long int)cb;
    task_pool[id].state = TASK_RUNNING;
    task_pool[id].priority = 1;
    task_pool[id].counter = TASK_COUNTER_NUM;
    task_pool[id].id = id;
    task_pool[id].resched = FALSE;
    task_queue_insert(&runq, id);
    return id;
}

void task_user_exec(int id, task_callback cb)
{
    if(id == TASK_INVALID_ID)
    {
        uart_printf("Warn: can't move invalid id to user mode\n\r");
    }
    PT_REGS *reg = (PT_REGS *)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    reg->sp = USER_STACK_BASE - id * USER_STACK_SIZE;
    reg->lr = (unsigned long int)cb;
    reg->pstate = PSR_MODE_EL0t;
}

void task_context_switch(TASK* next)
{
    TASK *prev = task_get_current();
    if(prev == next)
    {
        return;
    }
    task_switch_to(prev, next);
}

void task_set_state(TASK *cur, int state)
{
    cur->state = state;
}

void task_idle()
{
    int count = DELAY_COUNTER;
    while(1)
    {
        count--;
        if(count == 0)
        {
            count = DELAY_COUNTER;
            uart_printf("in idle task\n\r");
            task_schedule();
        }
    }
}

TASK *task_get_current()
{
    unsigned long int ptr;
    asm volatile("mrs %0, tpidr_el1" : "=r" (ptr));

    return (TASK *)ptr;
}

void task_resched()
{
    TASK *cur = task_get_current();
    if(cur->resched)
    {
        cur->resched = 0;
        task_schedule();
    }
}

int task_fork()
{
    TASK *parenttask = task_get_current();
    int parentid = parenttask->id;
    int childid = task_get_id();
    if(childid == TASK_INVALID_ID)
    {
        uart_printf("Warn: can't create privilege task\n\r");
        return -1;
    }
    TASK *childtask = &task_pool[childid];
    childtask->cpu_context.lr = (unsigned long int)task_ret_from_fork;
    childtask->cpu_context.fp = (unsigned long int)(KERNEL_STACK_BASE - childid * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    childtask->cpu_context.sp = childtask->cpu_context.fp;
    //
    childtask->kpc = (unsigned long int)NULL;
    childtask->state = TASK_RUNNING;
    childtask->priority = parenttask->priority;
    childtask->counter = parenttask->counter;
    childtask->id = childid;
    childtask->resched = parenttask->resched;
    task_queue_insert(&runq, childid);
    //
    PT_REGS *childreg = task_get_pt_regs(childid);
    PT_REGS *parentreg = task_get_pt_regs(parentid);
    childreg->regs[0] = 0;
    childreg->regs[29] = parentreg->regs[29];
    childreg->regs[30] = parentreg->regs[30];
    task_user_exec(childid, (task_callback)parentreg->lr);
    unsigned long int *childusp = (unsigned long int *)childreg->sp;
    unsigned long int *parentusp = (unsigned long int *)parentreg->sp;
    unsigned long int *parentsp_base = (unsigned long int *)(USER_STACK_BASE - parentid * USER_STACK_SIZE);
    while(parentsp_base > parentusp)
    {
        *childusp = *parentsp_base;
        childusp--;
        parentsp_base--;
    }
    childreg->sp = (unsigned long int)childusp;
    return childid;
}

PT_REGS *task_get_pt_regs(int id)
{
    return (PT_REGS *)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);;
}