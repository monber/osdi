#include "shell.h"

int main()
{
    uart_init(PL011);

    while(1)
    {
        shell_exec();
    }
    return 0;
}