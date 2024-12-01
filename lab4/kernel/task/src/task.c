#include "task.h"

TASK task_pool[TASK_POOL_NUM];
//char kstack_pool[TASK_POOL_NUM][KERNEL_STACK_SIZE];

void task_pool_init()
{
    //init task
    task_pool[INIT_TASK_ID].state = TASK_RUNNING;
    task_pool[INIT_TASK_ID].priority = 0;
    task_pool[INIT_TASK_ID].id = INIT_TASK_ID;
    task_pool[INIT_TASK_ID].counter = TASK_COUNTER_NUM;
    task_pool[INIT_TASK_ID].resched = FALSE;
    task_pool[INIT_TASK_ID].preempt = TRUE;
    task_queue_insert(&runq, INIT_TASK_ID);
    unsigned long int init_task_adr = (unsigned long int)(&task_pool[INIT_TASK_ID]);
    asm volatile("msr tpidr_el1, %0" : "=r" (init_task_adr));
    //other unused task
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

int task_create(unsigned int flag, void (*fn), unsigned long int arg)
{
    task_preempt_disable();
    int id = task_get_id();
    if(id == TASK_INVALID_ID)
    {
        printk("Warn: can't create privilege task\n\r");
        return TASK_INVALID_ID;
    }
    TASK *cur = &task_pool[id];
    //kernel/user context
    if(flag == PF_KERNEL)
    {
        cur->cpu_context.x19 = (unsigned long int)fn;
        cur->cpu_context.x20 = arg;
    }
    else if(flag == PF_USER)
    {
        cur->cpu_context.x19 = 0;
        task_user_exec(id, fn);
    }
    else
    {
        printk("Warn: unknown task create flag:%d\n\r", flag);
        return TASK_INVALID_ID;
    }
    //cpu context
    cur->cpu_context.lr = (unsigned long int)task_ret_from_fork;
    cur->cpu_context.fp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    cur->cpu_context.sp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
    //scheduler context
    cur->flag = flag;
    cur->state = TASK_RUNNING;
    cur->priority = 1;
    cur->counter = TASK_COUNTER_NUM;
    cur->id = id;
    cur->resched = FALSE;
    cur->preempt = TRUE;
    task_queue_insert(&runq, id);
    task_preempt_enable();
    return id;
}

int task_user_exec(int id, void(*fn))
{
    if(id == TASK_INVALID_ID)
    {
        printk("Warn: can't move invalid id to user mode\n\r");
        return -1;
    }
    PT_REGS *reg = task_get_pt_regs(id);
    reg->sp = USER_STACK_BASE - id * USER_STACK_SIZE;
    reg->lr = (unsigned long int)fn;
    reg->pstate = PSR_MODE_EL0t;
    task_pool[id].flag = PF_USER;
    return 0;
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

int task_user_fork()
{
    task_preempt_disable();
    TASK *parent = task_get_current();
    int parentid = parent->id;
    if(parent->flag != PF_USER)
    {
        printk("warn: task:%d is not a user task\n\r", parentid);
        task_preempt_enable();
        return -1;
    }
    PT_REGS *parentreg = task_get_pt_regs(parentid);
    int childid = task_create(PF_USER, (void *)parentreg->lr, 0);
    PT_REGS *childreg = task_get_pt_regs(childid);
    childreg->regs[0] = 0;
    childreg->regs[29] = parentreg->regs[29];
    childreg->regs[30] = parentreg->regs[30];
    unsigned long int *childusp = (unsigned long int *)childreg->sp;
    unsigned long int *parentusp = (unsigned long int *)parentreg->sp;
    unsigned long int *parentsp_base = (unsigned long int *)(unsigned long int)(USER_STACK_BASE - parentid * USER_STACK_SIZE);
    while(parentsp_base > parentusp)
    {
        *childusp = *parentsp_base;
        childusp--;
        parentsp_base--;
    }
    childreg->sp = (unsigned long int)childusp;
    task_preempt_enable();
    return childid;
}

PT_REGS *task_get_pt_regs(int id)
{
    if(id == TASK_INVALID_ID)
    {
        printk("Warn: invalid id:%d\n\r", id);
        return 0;
    }
    return (PT_REGS *)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE - PT_REGS_SIZE);
}

void task_exit(int status)
{
    task_preempt_disable();
    TASK *cur = task_get_current();
    cur->state = status;
    task_queue_delete(&runq, cur->id);
    task_preempt_enable();
    task_schedule();
}

void task_zombie_reaper()
{
    for(int i = 1; i < TASK_POOL_NUM; i++)
    {
        if(task_pool[i].state == TASK_ZOMBIE)
        {
            //To Do
            asm("nop");
        }
    }
}
