#include "shell.h"

static void shell_print_mbox_info()
{
    unsigned int board_revision = 0; 
    unsigned int mem_base_adr = 0;
    unsigned int mem_size = 0;
    mbox_get_board_revision(&board_revision);
    mbox_get_arm_memory_info(&mem_base_adr, &mem_size);
    char mbox_print_buf[MAX_BUFFER_SIZE];
    int2hex(board_revision, mbox_print_buf);
    uart_puts("board revision: ");
    uart_puts(mbox_print_buf);
    uart_puts("\n\r");
    //
    int2hex(mem_base_adr, mbox_print_buf);
    uart_puts("memory base address: ");
    uart_puts(mbox_print_buf);
    uart_puts("\n\r");
    //
    int2hex(mem_size, mbox_print_buf);
    uart_puts("memory size: ");
    uart_puts(mbox_print_buf);
    uart_puts("\n\r");
}

static void shell_print_timestamp()
{
    unsigned long int timer_count = 1;
    unsigned long int timer_frequency = 3;
    asm("mrs %0, CNTFRQ_EL0" : "=r" (timer_frequency));
    asm("mrs %0, CNTPCT_EL0" : "=r" (timer_count));
    float time = (float)timer_count / timer_frequency;
    char buffer[MAX_BUFFER_SIZE];
    ftoa(time, buffer);
    uart_send('[');
    uart_puts(buffer);
    uart_puts("]\n\r");
}

static void shell_cat()
{
    uart_puts("Filename: ");
    char filename[MAX_BUFFER_SIZE];
    strset(filename, 0, MAX_BUFFER_SIZE);
    shell_input(filename);
    char *filedata = (char *)cpio_get_file(filename);
    if(filedata)
    {
        uart_puts(filedata);
        uart_puts("\n\r");
    }
    else
    {
        uart_puts("\n\r");
        uart_puts("cannnot find ");
        uart_puts(filename);
        uart_puts("\n\r");
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
    uart_puts(str);
    uart_puts("\n\r");
    uart_puts(str2);
    uart_puts("\n\r");
}

static void shell_help()
{
    uart_puts("hello     : print Hello World\n\r");
    uart_puts("mailbox   : print info related to gpu\n\r");
    uart_puts("reboot    : reboot the device\n\r");
    uart_puts("timestamp : print timestamp in cpu\n\r");
    uart_puts("ls        : print files in rpi3\n\r");
    uart_puts("cat       : print data of search file\n\r");
    uart_puts("malloc    : print malloc string\n\r");
    uart_puts("fdt       : traverse device in rpi3\n\r");
}

void shell_exec()
{
    char buffer[MAX_BUFFER_SIZE];
    strset(buffer, 0, MAX_BUFFER_SIZE);

    //input cmd into shell
    uart_puts("# ");
    shell_input(buffer);

    //output result
    if(strcmp(buffer, "help") == 0)
    {
        shell_help();
    }
    else if(strcmp(buffer, "hello") == 0)
    {
        uart_puts("Hello World\n\r");
    }
    else if(strcmp(buffer, "mailbox") == 0)
    {
        shell_print_mbox_info();
    }
    else if(strcmp(buffer, "reboot") == 0)
    {
        uart_puts("cpu reboot...\n\r");
        reset(1000);
    }
    else if(strcmp(buffer, "timestamp") == 0)
    {
        shell_print_timestamp();
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
    else if(strlen(buffer) == MAX_BUFFER_SIZE)
    {
        uart_puts("\n\r");
        uart_puts("Cmd is too long to process.\n\r");
        uart_puts("Maximum input length is 128\n\r");
    }
    else
    {
        uart_puts("Error, command ");
        uart_puts(buffer);
        uart_puts(" not found. Please try <help>\n\r");
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