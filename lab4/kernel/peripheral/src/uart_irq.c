#include "uart_irq.h"

static void uart_tx_intr_hlr()
{
    while(!(*PL011_FR & 0x20))
    {
        char c = uart_buf_read(&uart_tx_buf);
        if(c)
        {
            *PL011_DR = c;
        }
        else
        {
            break;
        }
    }
    *PL011_ICR |= PL011_TX_INTR_CLEAR;
}

static void uart_rx_intr_hlr()
{
    while(!(*PL011_FR & 0x10))
    {
        char c = (char)(*PL011_DR);
        uart_buf_write(&uart_rx_buf, c);
    }
    *PL011_ICR |= PL011_RX_INTR_CLEAR;
}

void uart_intr_hlr()
{
    unsigned int reg = *PL011_RIS;
    if(reg & PL011_RX_INTR_STATUS)
    {
        //rx intr happen
        uart_rx_intr_hlr();
    }
    if(reg & PL011_TX_INTR_STATUS)
    {
        //tx intr happen
        uart_tx_intr_hlr();
    }
}