#include "shell.h"
#include "fdt.h"
#include "irq.h"
#include "sys.h"

int main()
{
    //uart_init(MINI_UART);
    fdt_traversal(fdt_initramfs_cb);
    
    //printf("kernel init \n\r");
    while(1)
    {
        shell_exec();
    }
    return 0;
}