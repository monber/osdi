#ifndef EXC_H_
#define EXC_H_
#include "uart.h"

//structure for ESR_EL2
typedef union esr_el2
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
}ESR_EL2;

void exc_print_info();
void exc_set();
void exc_invalid_entry();

#endif