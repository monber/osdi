#include "task.h"
#include "sched.h"
#include "timer.h"
#include "sys.h"

int id1 = TASK_INVALID_ID;
int id2 = TASK_INVALID_ID;

void user_process_2()
{
    int count = DELAY_COUNTER;
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            uart_printf("in user process 2\n\r");
            count = DELAY_COUNTER;
        }
    }
}

void user_process_1()
{
    int count = DELAY_COUNTER;
    uart_printf("in user process 1, start fork\n\r");
    int id = fork();
    //exec(user_process_2);
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            if(id == 0)
            {
                uart_printf("in fork user process\n\r");
            }
            else
            {
                uart_printf("in original user process:%d\n\r", id);
            }
            count = DELAY_COUNTER;
        }
    }
}

void kernel_process_1()
{
    int count = DELAY_COUNTER;
    uart_printf("in kernel process 1\n\r");
    TASK *cur = task_get_current();
    task_user_exec(cur->id, user_process_1);
    /*
    while(1)
    {
        count--;
        asm("nop");
        if(count == 0)
        {
            uart_printf("in kernel process 1\n\r");
            count = DELAY_COUNTER;
        }
    }
    */
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
            uart_printf("in kernel process 2\n\r");
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
    //unsigned int id2 = privilege_task_create(kernel_process_2);
    uart_printf("start idle task\n\r");
    task_idle();
    return ;
}

