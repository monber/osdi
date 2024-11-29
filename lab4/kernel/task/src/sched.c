#include "sched.h"

TASK_QUEUE runq = {.size = 0};

void task_queue_insert(TASK_QUEUE *tq, unsigned int id)
{
    tq->queue[tq->size] = id;
    tq->size++;
}

void task_queue_delete(TASK_QUEUE *tq, unsigned int id)
{
    for(int i = 0; i < tq->size; i++)
    {
        if(tq->queue[i] == id)
        {
            tq->queue[i] = tq->queue[tq->size - 1];
            tq->size--;
        }
    }
}

void task_schedule()
{
    TASK *task;
    unsigned int id;
    do
    {
        unsigned int priority = 0;
        for(int i = 0; i < runq.size; i++)
        {
            unsigned int curid = runq.queue[i];
            TASK *cur = &task_pool[curid];
            if(cur->priority >= priority)
            {
                priority = cur->priority;
                id = curid;
            }
            if(curid != TASK_IDLE_ID)
            {
                cur->priority++;
            }
        }
        task = &task_pool[id];
        if(id != TASK_IDLE_ID)
        {
            task->priority = 1;
            break;
        }
        task->priority = 0;
        for(int i = 0; i < runq.size; i++)
        {
            unsigned int curid = runq.queue[i];
            TASK *cur = &task_pool[curid];
            cur->counter = TASK_COUNTER_NUM;
        }
    }while(0);
    //uart_printf("context switch\n\r");
    task_context_switch(&task_pool[id]);
}

void task_timer_tick()
{
    TASK *cur = task_get_current();
    cur->counter--;
    if(cur->counter > 0 || !cur->preemption)
    {
        return ;
    }
    //uart_printf("schedule\n\r");
    irq_enable();
    task_schedule();
    irq_disable();
}

void task_preemption_enable()
{
    TASK *cur = task_get_current();
    cur->preemption = TRUE;
}

void task_preemption_disable()
{
    TASK *cur = task_get_current();
    cur->preemption = FALSE;
}