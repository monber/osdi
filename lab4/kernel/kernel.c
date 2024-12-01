#include "task.h"
#include "sched.h"
#include "timer.h"
#include "sys.h"
#include "uart.h"

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
  printf("Task %d after exec, tmp address 0x%x, tmp value %d\n\r", get_task_id(), &tmp, tmp);
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
            printf("Task id: %d, cnt: %d\n\r", get_task_id(), cnt);
            delay(1000000);
            ++cnt;
        }
        exit(TASK_ZOMBIE);
        printf("Should not be printed\n");
    }
    else
    {
        printf("Task %d before exec, cnt address 0x%x, cnt value %d\n\r", get_task_id(), &cnt, cnt);
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
    task_preempt_disable();
    int num_runnable_tasks = 0;
    for (int i = 0; i < TASK_POOL_NUM; i++)
    {
        if(task_pool[i].state == TASK_RUNNING)
        {
            num_runnable_tasks++;
        }
    }
    task_preempt_enable();
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
    printk("Test finished\n\r");
    while(1);
}

void kernel_main()
{
    uart_init(PL011);
    task_pool_init();
    core0_timer_enable();
    printf("kernel init \n\r");
    task_create(PF_KERNEL, user_test, 0);
    idle();
    return ;
}

