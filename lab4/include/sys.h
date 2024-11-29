#ifndef SYS_H_
#define SYS_H_

#include "syscall_table.h"

int exec(void (*func));
int fork();
#endif