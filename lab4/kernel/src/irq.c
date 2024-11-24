#include "irq.h"

void irq_entry()
{
    unsigned int core_isr_src = *CORE_ISR_SRC;
    unsigned int irq_pending2 = *IRQ_PENDING_2;
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

//used in EL2 init, doesn't need for now
void irq_el2_init()
{
    unsigned long int hcr_el2 = 0;
    unsigned long int daif = 0;
    //
    asm("mrs %0, HCR_EL2" : "=r" (hcr_el2));
    hcr_el2 |= (1 << 4);
    asm("msr HCR_EL2, %0" : "=r" (hcr_el2));
    //
    asm("mrs %0, DAIF" : "=r" (daif));
    daif &= ~(0xf << 6);
    asm("msr DAIF, %0" : "=r" (daif));
}

