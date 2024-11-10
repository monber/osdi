#ifndef CPIO_H_
#define CPIO_H_

#include "string.h"
#include "uart.h"
#include "utils.h"

//#define CPIO_BASE_ADR 0x8000000 //for QEMU
#define CPIO_ALIGNMENT 4
#define CPIO_END_FILENAME "TRAILER!!!"

extern void * CPIO_BASE_ADR;

typedef struct cpio_header 
{
    char    c_magic[6];
    char    c_ino[8];
    char    c_mode[8];
    char    c_uid[8];
    char    c_gid[8];
    char    c_nlink[8];
    char    c_mtime[8];
    char    c_filesize[8];
    char    c_devmajor[8];
    char    c_devminor[8];
    char    c_rdevmajor[8];
    char    c_rdevminor[8];
    char    c_namesize[8];
    char    c_check[8];
}CPIO_HEADER;

void cpio_ls();
void *cpio_get_file(const char *name);

#endif