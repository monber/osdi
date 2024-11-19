#ifndef IRQ_H_
#define IRQ_H_

#include "uart.h"
#include "timer.h"
#include "define.h"

#define CORE_ISR_SRC    ((volatile unsigned int *) 0x40000060)

void irq_hlr2();
void irq_init();

#endif