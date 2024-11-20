#include "irq.h"

void irq_hlr2()
{
    unsigned int core_isr_src = *CORE_ISR_SRC;
#if LOG_IRQ_HDR
    uart_puts("irq: ");
    uart_put_hex(core_isr_src);
    uart_puts("\n\r");
#endif
    if(core_isr_src & (1 << 11))
    {
        local_timer_handler();
    }
    if(core_isr_src & (1 << 1))
    {
        core0_timer_handler();
    }
    if(irq_pending2 & (1 << 25))
    {
        uart_intr_hlr();
    }
}

void irq_init()
{
    unsigned long int hcr_el2 = 0;
    unsigned long int daif = 0;
    asm("mrs %0, HCR_EL2" : "=r" (hcr_el2));
    hcr_el2 |= (1 << 4);
    asm("msr HCR_EL2, %0" : "=r" (hcr_el2));
    asm("mrs %0, DAIF" : "=r" (daif));
    daif &= ~(1 << 7);
    asm("msr DAIF, %0" : "=r" (daif));
}