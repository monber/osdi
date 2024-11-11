#ifndef STRING_H_
#define STRING_H_

int strcmp(const char *str1, const char *str2);
int strncmp(char *str1, char *str2, unsigned int n);
char *strcpy(char *dest, char *src);
int strlen(const char *str);
void strset(char *dest, int val, int size);
void itoa(int x, char *str);
void int2hex(unsigned int val, char *str);
void ftoa(float val, char *str);

#endif