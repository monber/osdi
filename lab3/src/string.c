#include "string.h"

int strcmp(const char *str1, const char *str2)
{
    if(!str1 || !str2)
    {
        return 0;
    }

    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    unsigned char c1, c2;
    
    do{
        c1 = (unsigned char)*s1;
        c2 = (unsigned char)*s2;
        s1++;
        s2++;
        if(c1 == '\0')
        {
            return c1 - c2;
        }
    }while(c1 == c2);
    
    return c1 - c2;
}

int strncmp(char *str1, char *str2, unsigned int n)
{
    if(!str1 || !str2)
    {
        return 0;
    }
    unsigned char *p1 = (unsigned char *)str1;
    unsigned char *p2 = (unsigned char *)str2;
    while(n > 0)
    {
        if(*p1 == '\0' || *p1 != *p2)
        {
            return *p1 - *p2;
        }
        p1++;
        p2++;
        n--;
    }
    return 0;
}

char *strcpy(char *dest, char *src)
{
    char *strdst = dest;
    if(!dest || !src)
    {
        return 0;
    }
    while(*src != '\0')
    {
        *strdst = *src;
        strdst++;
        src++;
    }
    return  dest;
}

int strlen(const char *str)
{
    if(!str)
    {
        return 0;
    }
    int count = 0;
    while(*(str + count) != '\0')
    {
        count++;
    }
    return count;
}

void strset(char *dest, int val, int size)
{
    if(!dest)
    {
        return;
    }
    char *cur = dest;
    while(size > 0)
    {
        *cur = val;
        cur++;
        size--;
    }
}

void reversestr(char *str)
{
    if(!str)
    {
        return;
    }
    int size = strlen(str);
    for(int i = 0; i < (size + 1) / 2; i++)
    {
        char tmp = str[i];
        str[i] = str[size - i - 1];
        str[size - i - 1] = tmp;
    }
}

void itoa(int x, char *str)
{
    int flag = (x >= 0) ? 1 : -1;
    int i = 0;
    x = (x > 0) ? x : -x;
    do
    {
        str[i] = (x % 10) + '0';
        x /= 10;
        i++;
    }while(x > 0);
    if(flag == - 1)
    {
        str[i] = '-';
        i++;
    }
    str[i] = '\0';
    reversestr(str);
}

void int2hex(unsigned long int val, char *str)
{
    char map[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int i = 0;
    do
    {
        int mod = val % 16;
        str[i] = (mod >= 10) ? map[mod - 10] : mod + '0';
        val /= 16;
        i++;
    }while(val > 0);
    str[i] = 'x';
    str[i + 1] = '0';
    str[i + 2] = '\0';
    reversestr(str);
}

void ftoa(float val, char *str)
{
    int intval = (int)val;
    itoa(intval, str);
    int intsize = strlen(str);
    val -= (float)intval;
    str[intsize] = '.';
    intsize++;
    do
    {
        val *= 10;
        intval = (int)val;
        str[intsize] = intval + '0';
        val -= (float)intval;
        intsize++;
    }while(val != 0);
    str[intsize] = '\0';
}