#include "exc.h"

void exc_entry()
{
    ESR_EL esr_el1;
    asm("mrs %0, ESR_EL1" : "=r" (esr_el1.reg));
#if LOG_EXC_ENTRY
    uart_puts("svc num: ");
    uart_put_hex(esr_el1.ISS);
    uart_puts("\n\r");
#endif
    switch(esr_el1.ISS)
    {
        case SYS_CALL_TIMESTAMP:
        {
            timer_print_timestamp();
            break;
        }
        case SYS_CALL_EXC:
        {
            exc_print_reg_info();
            break;
        }
        case SYS_CALL_TIMER:
        {
            timer_toggle();
            break;
        }
        case SYS_CALL_IRQ_ENABLE:
        {
            unsigned long int spsr_el1 = 0;
            asm("mrs %0, spsr_el1" : "=r" (spsr_el1));
            spsr_el1 &= ~(0xf << 6);
            asm("msr spsr_el1, %0" : "=r" (spsr_el1));
            break;
        }
        default:
        {
            uart_puts("unknown svc_num ");
            uart_put_hex(esr_el1.ISS);
            uart_puts("\n\r");
            exc_print_reg_info();
        }
    }
}

void exc_print_reg_info()
{
    unsigned long int elr_el1;
    ESR_EL esr_el1;
    asm("mrs %0, ELR_EL1" : "=r" (elr_el1));
    asm("mrs %0, ESR_EL1" : "=r" (esr_el1.reg));

    uart_puts("Return Address:");
    uart_put_hex(elr_el1);
    uart_puts(", EC:");
    uart_put_hex(esr_el1.EC);
    uart_puts(", ISS:");
    uart_put_hex(esr_el1.ISS);
    uart_puts("\n\r");
}

void exc_invalid_entry()
{
    uart_puts("Warn: enter invalid entry\n\r");
    exc_print_reg_info();
}

void exc_get_currentEL()
{
    unsigned long int currentEL;
    asm volatile("mrs %0, currentEL" : "=r" (currentEL));
    uart_printf("currentEL:%d\n\r", ((currentEL & 0xc) >> 2));
}