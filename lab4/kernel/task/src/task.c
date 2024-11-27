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

int privilege_task_create(task_callback cb)
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

void task_move_to_user_mode(int id, task_callback cb)
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

void do_exec(task_callback cb)
{
    int id = privilege_task_create(NULL);
    task_move_to_user_mode(id, cb);
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