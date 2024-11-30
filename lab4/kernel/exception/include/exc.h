#ifndef EXC_H_
#define EXC_H_
#include "uart.h"
#include "timer.h"
#include "sys.h"
#include "task.h"
#include "utils.h"

#define ESR_ELX_SVC64 0x15

typedef union esr_el
{
    unsigned long int reg;
    struct
    {
        unsigned long int ISS : 25;
        unsigned long int IL : 1;
        unsigned long int EC : 6;
        unsigned long int ISS2 : 5;
        unsigned long int RSVD : 27;
    };
}ESR_EL;

void exc_print_reg_info();
void exc_invalid_entry();
void exc_entry();
void exc_svc_handler(unsigned long int ISS);
void exc_get_currentEL();

#endif