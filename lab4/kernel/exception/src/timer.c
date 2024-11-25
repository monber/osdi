#include "timer.h"

bool timer_enable = FALSE;

static unsigned int local_timer_count = 0;
static unsigned int core_timer_count = 0;

void local_timer_enable()
{
  unsigned int flag = 0x30000000; // enable timer and interrupt.
  unsigned int reload = 50000000;
  set_reg(LOCAL_TIMER_CONTROL_REG, flag | reload);
  uart_printf("enable local timer\n\r");
}

void local_timer_disable()
{
  // disable timer and interrupt.
  set_reg(LOCAL_TIMER_CONTROL_REG, 0);
  uart_printf("disable local timer\n\r");
}

void local_timer_handler()
{
  set_reg(LOCAL_TIMER_IRQ_CLR, 0xc0000000); // clear interrupt and reload.
  local_timer_count++;
  uart_printf("local timer interrupt: %d\n\r", local_timer_count);
}

void core0_timer_enable()
{
  // enable timer
  unsigned long int cntp_ctl_el0 = 0;
  asm volatile("mrs %0, cntp_ctl_el0" : "=r" (cntp_ctl_el0));
  cntp_ctl_el0 |= 1;
  asm volatile("msr cntp_ctl_el0, %0" : "=r" (cntp_ctl_el0));
  //set expired time
  unsigned long int cntp_tval_el0 = CORE0_TIMER_EXPIRE_PERIOD;
  asm volatile("msr cntp_tval_el0, %0" : "=r" (cntp_tval_el0));
  //enable timer interrupt
  unsigned int core0_timer_irq_ctrl = read_reg(CORE0_TIMER_IRQ_CTRL);
  core0_timer_irq_ctrl |= 2;
  set_reg(CORE0_TIMER_IRQ_CTRL, core0_timer_irq_ctrl);
  uart_printf("enable core timer\n\r");
}

void core0_timer_disable()
{
  // enable timer
  unsigned long int cntp_ctl_el0 = 0;
  asm volatile("mrs %0, cntp_ctl_el0" : "=r" (cntp_ctl_el0));
  cntp_ctl_el0 &= ~1;
  asm volatile("msr cntp_ctl_el0, %0" : "=r" (cntp_ctl_el0));
  //enable timer interrupt
  unsigned int core0_timer_irq_ctrl = read_reg(CORE0_TIMER_IRQ_CTRL);
  core0_timer_irq_ctrl &= ~2;
  set_reg(CORE0_TIMER_IRQ_CTRL, core0_timer_irq_ctrl);
  uart_printf("disable core timer\n\r");
}

void core0_timer_handler()
{
  unsigned long int cntp_tval_el0 = CORE0_TIMER_EXPIRE_PERIOD;
  asm("msr cntp_tval_el0, %0" : "=r" (cntp_tval_el0));
  /*
  core_timer_count++;
  uart_printf("core timer interrupt: %d\n\r", core_timer_count);
  */
  task_timer_tick();
}

void timer_toggle()
{
    timer_enable ^= 1;
    if(timer_enable)
    {
        //local_timer_enable();
        core0_timer_enable();
    }
    else
    {
        //local_timer_disable();
        core0_timer_disable();
    }
}

void timer_print_timestamp()
{
  unsigned long int timer_count = 0;
  unsigned long int timer_frequency = 0;
  asm("mrs %0, CNTFRQ_EL0" : "=r" (timer_frequency));
  asm("mrs %0, CNTPCT_EL0" : "=r" (timer_count));
  float time = (float)timer_count / timer_frequency;
  char buffer[MAX_BUFFER_SIZE];
  ftoa(time, buffer);
  uart_printf("[%s]\n\r", buffer);
}