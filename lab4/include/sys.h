#ifndef SYS_H_
#define SYS_H_

#include "syscall_table.h"

int exec(void (*func));
int fork();
void exit(int status);
int get_task_id();
#endif