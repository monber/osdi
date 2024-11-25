#ifndef IRQ_H_
#define IRQ_H_

#include "uart.h"
#include "timer.h"
#include "define.h"
#include "utils.h"
#include "uart_irq.h"

#define CORE_ISR_SRC    ((volatile unsigned int *) 0x40000060)
#define PBASE           (0x3F000000)
#define IRQ_PENDING_2   ((volatile unsigned int *)(PBASE+0x0000B208))
#define IRQ_ENABLE_2    ((volatile unsigned int *)(PBASE+0x0000B214))

void irq_entry();
void irq_el2_init();

extern void irq_enable();
extern void irq_disable();

#endif