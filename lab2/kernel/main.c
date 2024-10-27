#include "shell.h"

int main()
{
    uart_init();
    uart_puts("hello world\n");
    while(1)
    {
        shell_exec();
    }
    return 0;
}