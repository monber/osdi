#include "shell.h"

#define USE_PL011 0

int main()
{
    uart_init(MINI_UART);
    uart_puts("hello world\n");
    while(1)
    {
        shell_exec();
    }
    return 0;
}