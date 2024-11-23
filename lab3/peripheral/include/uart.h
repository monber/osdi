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
#ifndef UART_H_
#define UART_H_

#include "gpio.h"
#include "mbox.h"
#include "string.h"
#include "uart_irq.h"
#include "irq.h"
#include "sprintf.h"

/* Auxilary mini UART registers */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

/* PL011 registers */
#define PL011_DR        ((volatile unsigned int*)(MMIO_BASE + 0x201000))
#define PL011_FR        ((volatile unsigned int*)(MMIO_BASE + 0x201018))
#define PL011_IBRD      ((volatile unsigned int*)(MMIO_BASE + 0x201024))
#define PL011_FBRD      ((volatile unsigned int*)(MMIO_BASE + 0x201028))
#define PL011_LCRH      ((volatile unsigned int*)(MMIO_BASE + 0x20102c))
#define PL011_CR        ((volatile unsigned int*)(MMIO_BASE + 0x201030))
#define PL011_IMSC      ((volatile unsigned int*)(MMIO_BASE + 0x201038))
#define PL011_RIS       ((volatile unsigned int*)(MMIO_BASE + 0x20103c))
#define PL011_ICR       ((volatile unsigned int*)(MMIO_BASE + 0x201044))

/* PL011 interrupt status */
#define PL011_TX_INTR_CLEAR     (1 << 5)
#define PL011_RX_INTR_CLEAR     (1 << 4)
#define PL011_TX_INTR_STATUS    (1 << 5)
#define PL011_RX_INTR_STATUS    (1 << 4)

void uart_init(int uart_mode);
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(char *s);
void uart_put_hex(unsigned long int hex);
void uart_put_int(int x);
void uart_printf(char *s, ...);

typedef enum uart_mode
{
    MINI_UART = 0,
    PL011 = 1,
}UART_MODE;

#endif