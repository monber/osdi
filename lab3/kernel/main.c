#include "shell.h"
#include "fdt.h"
#include "irq.h"

#define USE_PL011 0

int kernel_main()
{
    uart_init(PL011);
    fdt_traversal(fdt_initramfs_cb);
    irq_init();
    uart_puts("kernel init \n\r");
    while(1)
    {
        shell_exec();
    }
    return 0;
}