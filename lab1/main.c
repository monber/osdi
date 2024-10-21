#include "shell.h"

int main()
{
    uart_init();

    while(1)
    {
        shell_exec();
    }
    return 0;
}