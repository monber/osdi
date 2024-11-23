#include "shell.h"

static void shell_print_mbox_info()
{
    unsigned int board_revision = 0; 
    unsigned int mem_base_adr = 0;
    unsigned int mem_size = 0;
    mbox_get_board_revision(&board_revision);
    mbox_get_arm_memory_info(&mem_base_adr, &mem_size);
    uart_printf("board revision:0x%x\n\r", board_revision);
    uart_printf("memory base address:0x%x\n\r", mem_base_adr);
    uart_printf("memory size:0x%x\n\r", mem_size);
}

static void shell_cat()
{
    uart_printf("Filename: ");
    char filename[MAX_BUFFER_SIZE];
    strset(filename, 0, MAX_BUFFER_SIZE);
    shell_input(filename);
    char *filedata = (char *)cpio_get_file(filename);
    if(filedata)
    {
        uart_printf("%s\n\r", filedata);
    }
    else
    {
        uart_printf("cannnot find %s\n\r", filename);
    }
}

static void shell_malloc()
{
    char *str = (char *)malloc(5);
    for(int i = 0; i < 5; i++)
    {
        str[i] = '0' + i;
    }
    str[5] = '\0';
    char *str2 = (char *)malloc(5);
    for(int i = 0; i < 5; i++)
    {
        str2[i] = 'a' + i;
    }
    str2[5] = '\0';
    uart_printf("%s\n\r", str);
    uart_printf("%s\n\r", str2);
}

static void shell_help()
{
    uart_printf("hello     : print Hello World\n\r");
    uart_printf("mailbox   : print info related to gpu\n\r");
    uart_printf("reboot    : reboot the device\n\r");
    uart_printf("timestamp : print timestamp in cpu\n\r");
    uart_printf("ls        : print files in rpi3\n\r");
    uart_printf("cat       : print data of search file\n\r");
    uart_printf("malloc    : print malloc string\n\r");
    uart_printf("fdt       : traverse device in rpi3\n\r");
    uart_printf("exc       : call exception\n\r");
    uart_printf("irq       : enable timer\n\r");

}

void shell_exec()
{
    char buffer[MAX_BUFFER_SIZE];
    strset(buffer, 0, MAX_BUFFER_SIZE);

    //input cmd into shell
    uart_printf("# ");
    shell_input(buffer);

    //output result
    if(strcmp(buffer, "help") == 0)
    {
        shell_help();
    }
    else if(strcmp(buffer, "hello") == 0)
    {
        uart_printf("Hello World\n\r");
    }
    else if(strcmp(buffer, "mailbox") == 0)
    {
        shell_print_mbox_info();
    }
    else if(strcmp(buffer, "reboot") == 0)
    {
        uart_printf("cpu reboot...\n\r");
        reset(1000);
    }
    else if(strcmp(buffer, "timestamp") == 0)
    {
        LAUNCH_SYS_CALL(ASM_SYS_CALL_TIMESTAMP);
    }
    else if (strcmp(buffer, "ls") == 0)
    {
        cpio_ls();
    }
    else if (strcmp(buffer, "cat") == 0)
    {
        shell_cat();
    }
    else if(strcmp(buffer, "malloc") == 0)
    {
        shell_malloc();
    }
    else if(strcmp(buffer, "fdt") == 0)
    {
        fdt_traversal(fdt_print_name_cb);
    }
    else if(strcmp(buffer, "exc") == 0)
    {
        LAUNCH_SYS_CALL(ASM_SYS_CALL_EXC);
    }
    else if(strcmp(buffer, "irq") == 0)
    {
        LAUNCH_SYS_CALL(ASM_SYS_CALL_TIMER);
    }
    else if(strlen(buffer) == MAX_BUFFER_SIZE)
    {
        uart_printf("\n\r");
        uart_printf("Cmd is too long to process.\n\r");
        uart_printf("Maximum input length is 128\n\r");
    }
    else
    {
        uart_printf("Error, command %s not found. Please try <help>\n\r", buffer);
    }
}

void shell_input(char *str)
{
    int n = 0;
    while(n < MAX_BUFFER_SIZE)
    {
        char c = uart_getc();
        uart_send(c);
        if(c == '\n')
        {
            uart_send('\r');
            break;
        }
        str[n] = c;
        n++;
    }
}