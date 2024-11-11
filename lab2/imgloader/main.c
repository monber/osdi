/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"
#include "string.h"
#include "mbox.h"

void shell_input(char *str)
{
    int n = 0;
    while(n < MAX_BUFFER_SIZE)
    {
        char c = uart_getc();
        uart_send(c);
        if(c == '\n')
        {
            uart_send('\r');
            break;
        }
        str[n] = c;
        n++;
    }
}

void load_img()
{
    int loadsize = 0;
    char *loadadr = (char *)0x80000;
    uart_puts("Rpi3 is ready. Please send kernel img size\n\r");
    while(1)
    {
        char c = uart_getc();
        if (c == '\n')
        {
            break;
        }
        else if (c < '0' || c >'9')
        {
            uart_puts("Input error. Please start again\n\r");
            return ;
        }
        loadsize = loadsize * 10 + c - '0';
    }
    uart_puts("Rpi3 starts to load img\n\r");
    while(loadsize > 0)
    {
        *loadadr = uart_getc();
        loadadr++;
        loadsize--;
    }
    uart_puts("load finish\n\r");
    //
    asm volatile (
        "mov x0, x10;"
        "mov x1, x11;"
        "mov x2, x12;"
        "mov x3, x13;"
        // we must force an absolute address to branch to
        // x30 is used as LR in AArch64
        "mov x30, 0x80000; ret"
    );
}

void main()
{
    // set up serial console
    uart_init(PL011);
    while(1)
    {
        char buffer[MAX_BUFFER_SIZE];
        strset(buffer, 0, MAX_BUFFER_SIZE);

        //input cmd into shell
        uart_puts("# ");
        shell_input(buffer);

        if(strcmp(buffer, "loadimg") == 0)
        {
            load_img();
        }
    }
}