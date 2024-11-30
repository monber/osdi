#include "task.h"
#include "sched.h"
#include "timer.h"
#include "sys.h"

void delay(unsigned long time)
{
    while(time > 0)
    {
        asm ("nop");
        time--;
    }
}

void foo()
{
  int tmp = 5;
  uart_printf("Task %d after exec, tmp address 0x%x, tmp value %d\n", get_task_id(), &tmp, tmp);
  exit(TASK_ZOMBIE);
}

void test() {
    int cnt = 1;
    if (fork() == 0)
    {
        fork();
        delay(1000000);
        fork();
        while(cnt < 10)
        {
            uart_printf("Task id: %d, cnt: %d\n", get_task_id(), cnt);
            delay(1000000);
            ++cnt;
        }
        exit(TASK_ZOMBIE);
        uart_printf("Should not be printed\n");
    }
    else
    {
        uart_printf("Task %d before exec, cnt address 0x%x, cnt value %d\n", get_task_id(), &cnt, cnt);
        exec(foo);
    }
}

void user_test()
{
    TASK *cur = task_get_current();
    task_user_exec(cur->id, test);
}

int num_runnable_tasks()
{
    task_preemption_disable();
    int num_runnable_tasks = 0;
    for (int i = 0; i < TASK_POOL_NUM; i++)
    {
        if(task_pool[i].state == TASK_RUNNING)
        {
            num_runnable_tasks++;
        }
    }
    task_preemption_enable();
    return num_runnable_tasks;
}

void idle()
{
    while(1)
    {
        if(num_runnable_tasks() == 1)
        {
            break;
        }
        task_schedule();
        delay(1000000);
    }
    sys_uart_printf("Test finished\n");
    while(1);
}

void kernel_main()
{
    uart_init(PL011);
    task_pool_init();
    core0_timer_enable();
    sys_uart_printf("kernel init \n\r");
    privilege_task_create(user_test);
    idle();
    return ;
}

