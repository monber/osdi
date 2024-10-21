#include "shell.h"

void shell_exec()
{
    char buffer[MAX_BUFFER_SIZE];
    strset(buffer, 0, MAX_BUFFER_SIZE);
    //input cmd into shell
    shell_input(buffer);

    //output result
    if(strcmp(buffer, "help") == 0)
    {
        uart_puts("help       : print this help menu\n");
        uart_puts("hello      : print Hello World\n");
        uart_puts("reboot     : reboot the device\n");
        uart_puts("printInfo  : print info related to gpu\n");
    }
    else if(strcmp(buffer, "hello") == 0)
    {
        uart_puts("Hello World\n");
    }
    else if(strcmp(buffer, "printInfo") == 0)
    {
        mbox_print_board_revision();
        mbox_print_arm_memory_info();
    }
    else if(strcmp(buffer, "reboot") == 0)
    {
        uart_puts("cpu reboot\n");
        reset(100);
    }
    else
    {
        uart_puts("Error, command ");
        uart_puts(buffer);
        uart_puts(" not found. Please try <help>\n");
    }
}

void shell_input(char *str)
{
    uart_puts("# ");
    char *curs = str;
    while(1)
    {
        char c = uart_getc();
        uart_send(c);
        if(c == '\n')
        {
            break;
        }
        *curs = c;
        curs++;
    }
}