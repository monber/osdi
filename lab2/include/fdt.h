#ifndef FDT_H_
#define FDT_H_

#include "utils.h"
#include "string.h"
#include "uart.h"
#include "cpio.h"

#define FDT_MAGIC_WORD 0xd00dfeed
#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009
#define FDT_ALIGNEMENT_32 4

typedef struct fdt_header {
    unsigned int magic;
    unsigned int totalsize;
    unsigned int off_dt_struct;
    unsigned int off_dt_strings;
    unsigned int off_mem_rsvmap;
    unsigned int version;
    unsigned int last_comp_version;
    unsigned int boot_cpuid_phys;
    unsigned int size_dt_strings;
    unsigned int size_dt_struct;
}FDT_HEADER;

void fdt_traversal();
void fdt_init();

typedef void (*fdt_callback)(FDT_HEADER*, char*, unsigned int);
void fdt_initramfs_cb(FDT_HEADER *fdt, char *ptr, unsigned int token);
void fdt_print_name_cb(FDT_HEADER *fdt, char *ptr, unsigned int token);

extern void * CPIO_BASE_ADR;
#endif