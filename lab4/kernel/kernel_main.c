#include "task.h"

int id1 = TASK_INVALID_ID;
int id2 = TASK_INVALID_ID;

void kernel_process_1()
{
    int count = 10000000;
    uart_printf("in process 1\n\r");
    while(count > 0)
    {
        asm("nop");
        count--;
    }
    task_context_switch(&task_pool[id2]);
}

void kernel_process_2()
{
    int count = 10000000;
    uart_printf("in process 2\n\r");
    while(count > 0)
    {
        asm("nop");
        count--;
    }
    task_context_switch(&task_pool[id1]);
}

void kernel_main()
{
    uart_init(MINI_UART);
    uart_printf("kernel init \n\r");
    id1 = task_privilege_create(kernel_process_1);
    id2 = task_privilege_create(kernel_process_2);
    task_run((TASK *)&task_pool[id1]);
    while(1)
    {
        asm("nop");
    }
    return ;
}

