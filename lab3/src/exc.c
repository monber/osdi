#include "exc.h"

void exc_set()
{
    asm("svc #3");
}

void exc_print_info()
{
    unsigned long int elr_el2;
    ESR_EL2 esr_el2;
    asm("mrs %0, ELR_EL2" : "=r" (elr_el2));
    asm("mrs %0, ESR_EL2" : "=r" (esr_el2.reg));

    uart_puts("Return Address:");
    uart_put_hex(elr_el2);
    uart_puts(", EC:");
    uart_put_hex(esr_el2.EC);
    uart_puts(", ISS:");
    uart_put_hex(esr_el2.ISS);
    uart_puts("\n\r");
}

void exc_invalid_entry()
{
    uart_puts("Warn: enter invalid entry\n\r");
    exc_print_info();
}