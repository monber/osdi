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
int uart_mode = MINI_UART;
/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init(int uart_mode_)
{
    uart_mode = uart_mode_;
    register unsigned int r;
    if(uart_mode == MINI_UART)
    {
        /* initialize UART */
        *AUX_ENABLE |= 1;       // enable UART1, AUX mini uart
        *AUX_MU_CNTL = 0;
        *AUX_MU_IER = 0;
        *AUX_MU_LCR = 3;       // 8 bits
        *AUX_MU_MCR = 0;
        *AUX_MU_BAUD = 270;    // 115200 baud
        *AUX_MU_IIR = 0xc6;    // disable interrupts
    }
    else if(uart_mode == PL011)
    {
        *PL011_CR = 0;         // turn off UART0
        mbox_set_clkrate();
    }
    /* map UART1 to GPIO pins */
    r = *GPFSEL1;
    r &= ~((7<<12)|(7<<15)); // gpio14, gpio15
    if(uart_mode == MINI_UART)
    {
        r |= (2<<12)|(2<<15);    // alt5
    }
    else if(uart_mode == PL011)
    {
        r |= (4<<12)|(4<<15);    // alt0
    }
    *GPFSEL1 = r;
    *GPPUD = 0;            // enable pins 14 and 15
    r=150;
    while(r--)
    { 
        asm volatile("nop"); //delay 1 cycle
    }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150;
    while(r--)
    {
        asm volatile("nop");
    }
    *GPPUDCLK0 = 0;        // flush GPIO setup
    if(uart_mode == MINI_UART)
    {
        *AUX_MU_CNTL = 3;      // enable Tx, Rx
    }
    else if(uart_mode == PL011)
    {
        *PL011_ICR = 0x7FF;    // clear interrupts
        *PL011_IBRD = 0x2;
        *PL011_FBRD = 0xb;
        *PL011_LCRH = 0x7 << 4;
        *PL011_CR = 0x301;
    }
}

/**
 * Send a character
 */
void uart_send(unsigned int c) {
    switch(uart_mode)
    {
        case MINI_UART:
        {
            /* wait until we can send */
            do
            {
                asm volatile("nop");
            }while(!(*AUX_MU_LSR & 0x20));
            /* write the character to the buffer */
            *AUX_MU_IO = c;
            break;
        }
        case PL011:
        {
            do
            {
                asm volatile("nop");
            }while(*PL011_FR & 0x20);
            *PL011_DR = c;
            break;
        }
    }
}

/**
 * Receive a character
 */
char uart_getc() {
    char r = '0';
    switch(uart_mode)
    {
        case MINI_UART:
        {
            /* wait until something is in the buffer */
            do
            {
                asm volatile("nop");
            }while(!(*AUX_MU_LSR & 0x01));
            /* read it and return */
            r=(char)(*AUX_MU_IO);
            /* convert carriage return to newline */
            break;
        }
        case PL011:
        {
            do
            {
                asm volatile("nop");
            }while(*PL011_FR & 0x010);
            /* read it and return */
            r=(char)(*PL011_DR);
            /* convert carriage return to newline */
            break;
        }
    }
    
    return r == '\r' ? '\n' : r;
}

/**
 * Display a string
 */
void uart_puts(char *s) {
    while(*s) {
        /* convert newline to carriage return + newline */
        uart_send(*s++);
    }
}

void uart_put_hex(unsigned long int hex)
{
    char s[MAX_BUFFER_SIZE];
    int2hex(hex, s);
    uart_puts(s);
}

void uart_put_int(int x)
{
    char s[MAX_BUFFER_SIZE];
    itoa(x, s);
    uart_puts(s);
}