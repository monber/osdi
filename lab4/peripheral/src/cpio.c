#include "cpio.h"
void * CPIO_BASE_ADR = NULL;
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

static unsigned long cpio_align(unsigned long n, unsigned long align)
{
    return (n + align - 1) & (~(align - 1));
}
/* return 1 for EOF, 0 for normal, -1 for wrong magic word */
static int cpio_parse_header(CPIO_HEADER *archive, char **filename, void **data, CPIO_HEADER **next)
{
    if(strncmp(archive->c_magic, CPIO_MAGIC_WORD, sizeof(archive->c_magic)) != 0)
    {
        uart_printf("Warn: read abnormal cpio magic word\n\r");
        return -1;
    }
    *filename = (char *)archive + sizeof(CPIO_HEADER);
    if(strcmp(*filename, CPIO_END_FILENAME) == 0)
    {
        return 1;
    }
    unsigned long namesize = parse_hex_str(archive->c_namesize, sizeof(archive->c_namesize));
    *data = (void *)(cpio_align((unsigned long)*filename + namesize, CPIO_ALIGNMENT));
    unsigned long filesize = parse_hex_str(archive->c_filesize, sizeof(archive->c_filesize));
    *next = (CPIO_HEADER *)(cpio_align((unsigned long)*data + filesize, CPIO_ALIGNMENT));
    return 0;
}

void cpio_ls()
{
    CPIO_HEADER *header = (CPIO_HEADER *)CPIO_BASE_ADR;
    while(1)
    {
        char *filename = NULL;
        void *filedata = NULL;
        CPIO_HEADER *next = NULL;
        int error = cpio_parse_header(header, &filename, &filedata, &next);
        if(error)
        {
            break;
        }
        uart_printf("%s\n\r", filename);
        header = next;
    }
}

void *cpio_get_file(const char *name)
{
    CPIO_HEADER *header = (CPIO_HEADER *)CPIO_BASE_ADR;
    while(1)
    {
        char *filename = NULL;
        void *filedata = NULL;
        CPIO_HEADER *next = NULL;
        int error = cpio_parse_header(header, &filename, &filedata, &next);
        if(error)
        {
            break;
        }
        if(strcmp(name, filename) == 0)
        {
            return filedata;
        }
        header = next;
    }
    return NULL;
}