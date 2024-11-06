#include "malloc.h"

char *malloc_ptr = (char *)MALLOC_BASE_ADR;

void *malloc(unsigned int size)
{
    unsigned int memalign_size = align(size, 4);
    if ((malloc_ptr + memalign_size) <= (char *)(MALLOC_BASE_ADR + MALLOC_MAX_SIZE))
    {
        malloc_ptr += memalign_size;
        return (void *)malloc_ptr;
    }
    return 0;
}