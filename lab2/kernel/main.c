#include "shell.h"
#include "fdt.h"

#define USE_PL011 0

int main()
{
    /* fdt_init should be called first in main for value of x0 may change*/
    fdt_init();
    uart_init(MINI_UART);
    fdt_traversal(fdt_initramfs_cb);
    while(1)
    {
        shell_exec();
    }
    return 0;
}