#ifndef UART_IRQ_H_
#define UART_IRQ_H_

#include "uart.h"
#include "string.h"
#include "utils.h"

typedef struct uart_buf
{
    char buf[MAX_BUFFER_SIZE];
    int ridx;
    int widx;
}UART_BUF;

extern UART_BUF uart_tx_buf;
extern UART_BUF uart_rx_buf;

void uart_intr_hlr();
void uart_buf_init(UART_BUF *uart_buf);
void uart_buf_write(UART_BUF *uart_buf, char c);
char uart_buf_read(UART_BUF *uart_buf);
bool uart_buf_is_full(UART_BUF *uart_buf);
bool uart_buf_is_empty(UART_BUF *uart_buf);
void uart_buf_read_all(UART_BUF *uart_buf, char *s);

#endif