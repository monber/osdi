#include "utils.h"

#define MALLOC_BASE_ADR     0x3B000000
#define MALLOC_MAX_SIZE     0x100000

void *malloc(unsigned int size);