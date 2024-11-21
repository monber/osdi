#include "shell.h"
#include "fdt.h"
#include "irq.h"
#include "syscall.h"

int main()
{
    uart_init(PL011);
    fdt_traversal(fdt_initramfs_cb);
    
    uart_puts("kernel init \n\r");
    while(1)
    {
        shell_exec();
    }
    return 0;
}