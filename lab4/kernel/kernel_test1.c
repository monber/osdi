#include "task.h"
#include "sched.h"
#include "timer.h"
#include "sys.h"
#include "uart.h"

/*
void delay(unsigned long time)
{
    while(time > 0)
    {
        asm ("nop");
        time--;
    }
}

void foo(int i){
    while(1)
    {
        TASK* cur = task_get_current();
        printk("Task id: %d, arg:%d\n", cur->id, i);
        delay(1000000);
        //task_schedule();
    }
}

void idle(){
    while(1)
    {
        task_schedule();
        delay(1000000);
    }
}

void kernel_main_test1()
{
    uart_init(MINI_UART);
    task_pool_init();
    core0_timer_enable();
    printk("kernel init \n\r");
    for(int i = 0; i < 10; ++i) { // N should > 2
        task_create(PF_KERENL, foo, 0);
    }
    idle();
    return ;
}
*/
