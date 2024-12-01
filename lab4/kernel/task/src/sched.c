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
            if(curid != INIT_TASK_ID)
            {
                cur->priority++;
            }
        }
        task = &task_pool[id];
        if(id != INIT_TASK_ID)
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
    //printk("context switch\n\r");
    task_context_switch(&task_pool[id]);
}

void task_timer_tick()
{
    TASK *cur = task_get_current();
    cur->counter--;
    if(cur->counter > 0 || !cur->preempt)
    {
        return ;
    }
    //printk("schedule\n\r");
    irq_enable();
    task_schedule();
    irq_disable();
}

void task_preempt_enable()
{
    TASK *cur = task_get_current();
    cur->preempt = TRUE;
}

void task_preempt_disable()
{
    TASK *cur = task_get_current();
    cur->preempt = FALSE;
}