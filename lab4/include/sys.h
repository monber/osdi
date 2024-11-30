#ifndef SYS_H_
#define SYS_H_

#include "syscall_table.h"
#include "utils.h"
#include "string.h"
#include "sprintf.h"

int exec(void (*func));
int fork();
void exit(int status);
int get_task_id();
size_t uart_read(char buf[], size_t size);
size_t uart_write(const char buf[], size_t size);

#endif