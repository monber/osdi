#include "sys.h"

void uart_printf(char *s, ...)
{
    char dst[MAX_BUFFER_SIZE];
    __builtin_va_list args;
    __builtin_va_start(args, s);
    sprintf(dst, s, args);
    //uart_puts(dst);
    uart_write(dst, strlen(dst));
}