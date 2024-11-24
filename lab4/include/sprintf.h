#ifndef SPRINTF_H_
#define SPRINTF_H_

#include "utils.h"
#include "string.h"
#include "uart.h"

//  Function sprintf only support %d, %x, %c and %s for now.
unsigned int sprintf(char *dst, char *src, __builtin_va_list args);

#endif