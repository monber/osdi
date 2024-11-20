#ifndef UTILS_H_
#define UTILS_H_

#define PM_PASSWORD 0x5a000000
#define PM_RSTC 0x3F10001c
#define PM_WDOG 0x3F100024

#define MAX_BUFFER_SIZE 512
#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0

typedef unsigned char bool;

// reboot api
void set_reg(long addr, unsigned int value);
unsigned int read_reg(long addr);
void reset(int tick);
void cancel_reset();

//
unsigned int align(unsigned int size, unsigned int n);

#endif

