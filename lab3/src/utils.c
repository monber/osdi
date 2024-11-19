#include "utils.h"

void set_reg(long addr, unsigned int value)
{
    volatile unsigned int* point = (volatile unsigned int*)addr;
    *point = value;
}

unsigned int read_reg(long addr)
{
    volatile unsigned int* point = (volatile unsigned int*)addr;
    return *point;
}

void reset(int tick)
{                                          // reboot after watchdog timer expire
    set_reg(PM_RSTC, PM_PASSWORD | 0x20);  // full reset
    set_reg(PM_WDOG, PM_PASSWORD | tick);  // number of watchdog tick
}

void cancel_reset()
{
    set_reg(PM_RSTC, PM_PASSWORD | 0);  // full reset
    set_reg(PM_WDOG, PM_PASSWORD | 0);  // number of watchdog tick
}

unsigned int align(unsigned int size, unsigned int n)
{
    return (size + (n - 1)) & (~(n - 1));
}