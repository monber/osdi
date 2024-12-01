#ifndef TIMER_H_
#define TIMER_H_

#include "utils.h"
#include "uart.h"
#include "sched.h"

#define LOCAL_TIMER_CONTROL_REG 0x40000034
#define LOCAL_TIMER_IRQ_CLR 0x40000038
#define CORE0_TIMER_IRQ_CTRL 0x40000040
#define CORE0_TIMER_EXPIRE_PERIOD 0xffff

void local_timer_enable();
void local_timer_disable();
void local_timer_handler();
void core0_timer_enable();
void core0_timer_disable();
void core0_timer_handler();
void timer_toggle();
void timer_print_timestamp();

#endif