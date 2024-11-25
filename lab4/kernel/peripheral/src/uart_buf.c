#include "uart_buf.h"

UART_BUF uart_tx_buf;
UART_BUF uart_rx_buf;

void uart_buf_init(UART_BUF *uart_buf)
{
    strset(uart_buf->buf, 0, sizeof(uart_buf->buf));
    uart_buf->ridx = 0;
    uart_buf->widx = 0;
}

void uart_buf_write(UART_BUF *uart_buf, char c)
{
    if(!uart_buf_is_full(uart_buf))
    {
        uart_buf->buf[uart_buf->widx] = c;
        uart_buf->widx = (uart_buf->widx + 1) % MAX_BUFFER_SIZE;
    }
}

char uart_buf_read(UART_BUF *uart_buf)
{
    char ret = 0;
    if(!uart_buf_is_empty(uart_buf))
    {
        ret = uart_buf->buf[uart_buf->ridx];
        uart_buf->ridx = (uart_buf->ridx + 1) % MAX_BUFFER_SIZE;
    }
    return ret;
}

bool uart_buf_is_empty(UART_BUF *uart_buf)
{
    return (uart_buf->ridx == uart_buf->widx);
}

bool uart_buf_is_full(UART_BUF *uart_buf)
{
    int dif = (uart_buf->ridx + MAX_BUFFER_SIZE - uart_buf->widx) % MAX_BUFFER_SIZE;
    return (dif == 1);
}