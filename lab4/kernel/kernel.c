#include "task.h"
#include "sched.h"
#include "timer.h"

int id1 = TASK_INVALID_ID;
int id2 = TASK_INVALID_ID;

void kernel_process_1()
{
    int count = DELAY_COUNTER;
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            uart_printf("in process 1\n\r");
            count = DELAY_COUNTER;
        }
    }
}

void kernel_process_2()
{
    int count = DELAY_COUNTER;
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            uart_printf("in process 2\n\r");
            count = DELAY_COUNTER;
        }
    }
}

void kernel_process_3()
{
    int count = DELAY_COUNTER;
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            uart_printf("in process 3\n\r");
            count = DELAY_COUNTER;
        }
    }
}

void kernel_main()
{
    uart_init(MINI_UART);
    task_pool_init();
    core0_timer_enable();
    uart_printf("kernel init \n\r");
    unsigned int id1 = privilege_task_create(kernel_process_1);
    unsigned int id2 = privilege_task_create(kernel_process_2);
    unsigned int id3 = privilege_task_create(kernel_process_3);
    uart_printf("start idle task\n\r");
    task_idle();
    return ;
}

