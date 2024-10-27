#include "cpio.h"

static unsigned long cpio_align(unsigned long val, unsigned long align)
{
    return (val + align - 1) & (~(align - 1));
}

static unsigned long parse_hex_str(char *s, unsigned int len)
{
    unsigned long ret = 0;
    for(int i = 0; i < len; i++)
    {
        ret *= 16;
        if('0' <= s[i] && s[i] <= '9')
        {
            ret += s[i] - '0';
        }
        else if('a' <= s[i] && s[i] <= 'f')
        {
            ret += s[i] - 'a' + 10;
        }
        else if('A' <= s[i] && s[i] <= 'F')
        {
            ret += s[i] - 'A' + 10;
        }
        else
        {
            return ret;
        }
    }
    return ret;
}

static int cpio_strcmp(const char *s1, const char *s2, unsigned long size)
{
    for(unsigned long i = 0; i < size; i++)
    {
        if(s1[i] != s2[i])
        {
            return s1[i] - s2[i];
        }
        if(s1[i] == '0')
        {
            return 0;
        }
    }
    return 0;
}

void cpio_ls()
{
    char *curadr = (char *)cpio_adr;
    while(1)
    {
        if(strcmp((char *)(curadr + sizeof(CPIO_HEADER)), "TRAILER!!!") == 0)
        {
            break;
        }
        CPIO_HEADER *cpio_ptr = (CPIO_HEADER *)curadr;
        unsigned long namesize = parse_hex_str(cpio_ptr->c_namesize, sizeof(cpio_ptr->c_namesize));
        unsigned long filesize = parse_hex_str(cpio_ptr->c_filesize, sizeof(cpio_ptr->c_filesize));
        char *namec = (curadr + sizeof(CPIO_HEADER));
        for(unsigned long i = 0; i < namesize; i++)
        {
            uart_send(namec[i]);
        }
        uart_send('\n');
        curadr += cpio_align(sizeof(CPIO_HEADER) + namesize + filesize, CPIO_ALIGNMENT);
    }
}

void *cpio_get_file(const char *name, unsigned long *size)
{
    unsigned char *curadr = (unsigned char *)cpio_adr;
    while(1)
    {
        if(strcmp((char *)(curadr + sizeof(CPIO_HEADER)), "TRAILER!!!") == 0)
        {
            break;
        }
        CPIO_HEADER *cpio_ptr = (CPIO_HEADER *)curadr;
        unsigned long namesize = parse_hex_str(cpio_ptr->c_namesize, sizeof(cpio_ptr->c_namesize));
        unsigned long filesize = parse_hex_str(cpio_ptr->c_filesize, sizeof(cpio_ptr->c_filesize));
        if(strcmp((char *)(curadr + sizeof(CPIO_HEADER)), name) == 0)
        {
            *size = filesize;
            return (void *)(curadr + cpio_align((sizeof(CPIO_HEADER) + namesize), CPIO_ALIGNMENT));
        }
        curadr += cpio_align(sizeof(CPIO_HEADER) + namesize + filesize, CPIO_ALIGNMENT);
    }
    return 0;
}