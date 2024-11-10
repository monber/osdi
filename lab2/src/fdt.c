#include "fdt.h"

void * fdt_base_adr = NULL;


static unsigned int fdt32_be(const void* p)
{
    const unsigned char *buf = (const unsigned char *)p;
    unsigned int ret = (buf[0] << 24) |
                       (buf[1] << 16) |
                       (buf[2] << 8) |
                       (buf[3]);
    return ret;
}
/*
static unsigned long fdt64_be(const void* p)
{
    const unsigned char *buf = (const unsigned char *)p;
    unsigned long ret = (buf[0] << 56) |
                       (buf[1] << 48) |
                       (buf[2] << 40) |
                       (buf[3] << 32) |
                       (buf[4] << 24) |
                       (buf[5] << 16) |
                       (buf[6] << 8) |
                       (buf[7]);
    return ret;
}
*/
static int fdt_parse_struct(FDT_HEADER *fdt, char *ptr, char **next, fdt_callback cb)
{
    unsigned int token = fdt32_be((void *)ptr);
    cb(fdt, ptr, token);
    ptr += 4;
    switch(token)
    {
        case FDT_BEGIN_NODE:
        {
            int namesize = strlen(ptr) + 1;
            /*
            char s[MAX_BUFFER_SIZE];
            if(*ptr)
            {
                uart_puts(ptr);
                uart_puts("\n");
            }
            */
            /*
            itoa(namesize, s);
            uart_puts(s);
            uart_send('\n');
            */
            *next = ptr + align(namesize, FDT_ALIGNEMENT_32);
            //uart_puts("FDT_BEGIN_NODE\n\r");
            break;
        }
        case FDT_END_NODE:
        {
            *next = ptr;
            //uart_puts("FDT_END_NODE\n\r");
            break;
        }
        case FDT_NOP:
        {
            *next = ptr;
            //uart_puts("FDT_NOP\n\r");
            break;
        }
        case FDT_PROP:
        {
            //uart_puts("FDT_PROP\n\r");
            unsigned int len = fdt32_be(ptr);
            *next = ptr + 8 + align(len, FDT_ALIGNEMENT_32);
            break;
        }
        case FDT_END:
        {
            //uart_puts("FDT_END\n\r");
            *next = NULL;
            return 1;
        }
        default:
        {
            //uart_puts("unknown\n\r");
            *next = NULL;
            return 1;
        }
    }
    return 0;
}

void fdt_traversal(fdt_callback cb)
{
    FDT_HEADER *fdt = (FDT_HEADER *)fdt_base_adr;
    if(fdt32_be((void *)&fdt->magic) != FDT_MAGIC_WORD)
    {
        return ;
    }
    char *ptr = ((char *)fdt + fdt32_be(&fdt->off_dt_struct));
    while(1)
    {
        char *next = NULL;
        int err = fdt_parse_struct(fdt, ptr, &next, cb);
        if(err)
        {
            return;
        }
        ptr = next;
    }
}

void fdt_init()
{
    unsigned long int adr;
    asm("mov %0, x0" : "=r" (adr));
    fdt_base_adr = (void *)adr;
}

void fdt_initramfs_cb(FDT_HEADER *fdt, char *ptr, unsigned int token)
{
    if(token != FDT_PROP)
    {
        return;
    }
    unsigned int nameoff = fdt32_be(ptr + 8);
    char *string_ptr = (char *)fdt + fdt32_be((void *)&fdt->off_dt_strings) + nameoff;
    if(strcmp(string_ptr, "linux,initrd-start") == 0)
    {
        CPIO_BASE_ADR = (void *)(fdt32_be((void *)(ptr + 12)));
    }
}

void fdt_print_name_cb(FDT_HEADER *fdt, char *ptr, unsigned int token)
{
    if(token != FDT_BEGIN_NODE)
    {
        return ;
    }
    uart_puts(ptr + 4);
    uart_puts("\n\r");
}
