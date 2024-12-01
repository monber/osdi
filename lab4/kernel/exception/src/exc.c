#include "exc.h"

void exc_entry()
{
    ESR_EL esr_el1;
    asm("mrs %0, ESR_EL1" : "=r" (esr_el1.reg));
#if LOG_EXC_ENTRY
    uart_puts("svc num: ");
    uart_put_hex(esr_el1.ISS);
    uart_puts("\n\r");
#endif
    if(esr_el1.EC == ESR_ELX_SVC64)
    {
        exc_svc_handler(esr_el1.ISS);
    }
    else
    {
        printk("Warn: unknown exception\n\r");
        exc_print_reg_info();
    }
}

static void exc_sys_call_hdr()
{
    TASK *cur = task_get_current();
    PT_REGS *reg = task_get_pt_regs(cur->id);
    switch(reg->regs[8])
    {
        /*
        case SYS_CALL_TIMESTAMP:
        {
            timer_print_timestamp();
            break;
        }
        case SYS_CALL_EXC:
        {
            exc_print_reg_info();
            break;
        }
        case SYS_CALL_TIMER:
        {
            timer_toggle();
            break;
        }
        case SYS_CALL_IRQ_ENABLE:
        {
            unsigned long int spsr_el1 = 0;
            asm("mrs %0, spsr_el1" : "=r" (spsr_el1));
            spsr_el1 &= ~(0xf << 6);
            asm("msr spsr_el1, %0" : "=r" (spsr_el1));
            break;
        }
        */
        case SYS_CALL_EXC:
        {
            int ret = task_user_exec(cur->id, (void *)reg->regs[0]);
            reg->regs[0] = ret;
            break;
        }
        case SYS_CALL_FORK:
        {
            reg->regs[0] = (unsigned long int)task_user_fork();
            break;
        }
        case SYS_CALL_EXIT:
        {
            task_exit(reg->regs[0]);
            break;
        }
        case SYS_CALL_GET_TASK_ID:
        {
            reg->regs[0] = cur->id;
            break;
        }
        case SYS_CALL_UART_READ:
        {
            char *s = (char *)reg->regs[0];
            size_t size = reg->regs[1];
            reg->regs[0] = sys_uart_read(s, size);
            break;
        }
        case SYS_CALL_UART_WRITE:
        {
            char *s = (char *)reg->regs[0];
            size_t size = reg->regs[1];
            reg->regs[0] = sys_uart_write(s, size);
            break;
        }
        default:
        {
            printk("Warn: unknown syscall num:%d\n\r", reg->regs[8]);
        }
    }
}

void exc_svc_handler(unsigned long int ISS)
{
    if(ISS == SYS_CALL)
    {
        exc_sys_call_hdr();
    }
    else
    {
        printk("Warn: unknown svc_num:%d\n\r", ISS);
    }
}

void exc_print_reg_info()
{
    unsigned long int elr_el1;
    ESR_EL esr_el1;
    asm("mrs %0, ELR_EL1" : "=r" (elr_el1));
    asm("mrs %0, ESR_EL1" : "=r" (esr_el1.reg));

    printk("Return Address:0x%x, EC:0x%x, ISS:0x%x\n\r", elr_el1, esr_el1.EC, esr_el1.ISS);
}

void exc_invalid_entry()
{
    printk("Warn: enter invalid entry\n\r");
    exc_print_reg_info();
}

void exc_get_currentEL()
{
    unsigned long int currentEL;
    asm volatile("mrs %0, currentEL" : "=r" (currentEL));
    printk("currentEL:%d\n\r", ((currentEL & 0xc) >> 2));
}