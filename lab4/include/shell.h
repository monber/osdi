#include "string.h"
#include "uart.h"
#include "utils.h"
#include "mbox.h"
#include "cpio.h"
#include "malloc.h"
#include "fdt.h"
#include "exc.h"
#include "timer.h"
#include "syscall.h"

void shell_exec();
void shell_input(char *str);