#include "sprintf.h"

unsigned int sprintf(char *dst, char *src, __builtin_va_list args)
{
    unsigned long int arg = 0;
    char *str = NULL;
    if(!dst || !src)
    {
        return 0;
    }
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;
    char buf[MAX_BUFFER_SIZE];
    unsigned int count = 0;
    while(*s)
    {
        if(*s == '%')
        {
            s++;
            switch(*s)
            {
                case 'd':
                {
                    arg =  __builtin_va_arg(args, int);
                    itoa(arg, buf);
                    strcpy((char *)d, buf);
                    d += strlen(buf);
                    count += strlen(buf);
                    break;
                }
                case 'x':
                {
                    arg =  __builtin_va_arg(args, long int);
                    int2hex(arg, buf);
                    strcpy((char *)d, buf);
                    d += strlen(buf);
                    count += strlen(buf);
                    break;
                }
                case 'c':
                {
                    arg = __builtin_va_arg(args, int);
                    *d = (char)arg;
                    d++;
                    count++;
                    break;
                }
                case 's':
                {
                    str =  __builtin_va_arg(args, char *);
                    strcpy((char *)d, str);
                    d += strlen(str);
                    count += strlen(str);
                    break;
                }
            }
            s++;
            count -= 2;
        }
        else
        {
            *d = *s;
            d++;
            s++;
        }

    }
    *d = '\0';
    return count;
}