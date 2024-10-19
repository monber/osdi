#include "uart.h"
#include "string.h"
#include "mbox.h"

int main()
{
    uart_init();

    while(1)
    {
        //
        char buffer[MAX_BUFFER_SIZE];
        strset(buffer, 0, MAX_BUFFER_SIZE);
        uart_gets(buffer);
        if(strcmp(buffer, "help\n") == 0)
        {
            uart_puts("help       : print this help menu\n");
            uart_puts("hello      : print Hello World\n");
            uart_puts("reboot     : reboot the device\n");
        }
        else if(strcmp(buffer, "hello\n") == 0)
        {
            uart_puts("Hello World\n");
        }
        else if(strcmp(buffer, "print gpu info\n") == 0)
        {
            mbox_print_board_revision();
            mbox_print_arm_memory_info();
        }
        else if(strcmp(buffer, "reboot\n") == 0)
        {
            //To Do
            break;
        }
    }
    return 0;
}