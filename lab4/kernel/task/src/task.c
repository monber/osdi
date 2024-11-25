#include "task.h"

TASK task_pool[TASK_POOL_NUM];
//char kstack_pool[TASK_POOL_NUM][KERNEL_STACK_SIZE];

void task_poll_init()
{
    for(int i = 0; i < TASK_POOL_NUM; i++)
    {
        task_pool[i].state = TASK_IDLE;
        task_pool[i].cpu_context.fp = 0;
        task_pool[i].cpu_context.lr = 0;
        task_pool[i].cpu_context.pc = 0;
        task_pool[i].cpu_context.sp = 0;
        task_pool[i].cpu_context.x19 = 0;
        task_pool[i].cpu_context.x20 = 0;
        task_pool[i].cpu_context.x21 = 0;
        task_pool[i].cpu_context.x22 = 0;
        task_pool[i].cpu_context.x23 = 0;
        task_pool[i].cpu_context.x24 = 0;
        task_pool[i].cpu_context.x25 = 0;
        task_pool[i].cpu_context.x26 = 0;
        task_pool[i].cpu_context.x27 = 0;
        task_pool[i].cpu_context.x28 = 0;
    }
}

int task_privilege_create(task_callback cb)
{
    int id = 0;
    while(id < TASK_POOL_NUM)
    {
        if(task_pool[id].state == TASK_IDLE)
        {
            task_pool[id].state = TASK_USED;
            break;
        }
        id++;
    }
    if(id == TASK_INVALID_ID)
    {

        uart_printf("Warn: can't create privilege task\n\r");
        return TASK_INVALID_ID;
    }
    task_pool[id].cpu_context.lr = (unsigned long int)cb;
    task_pool[id].cpu_context.fp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE);
    task_pool[id].cpu_context.sp = (unsigned long int)(KERNEL_STACK_BASE - id * KERNEL_STACK_SIZE);
    //uart_printf("sp:0x%x\n\r", task_pool[id].cpu_context.sp);
    //uart_printf("create privilege task:%d success\n\r", id);
    return id;
}

void task_context_switch(TASK* next)
{
    TASK *prev = task_get_current();
    task_switch_to(prev, next);
}