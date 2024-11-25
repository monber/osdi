#ifndef SYSCALL_H_
#define SYSCALL_H_

#define SYS_CALL_TIMESTAMP  1
#define SYS_CALL_EXC        2
#define SYS_CALL_TIMER      3
#define SYS_CALL_IRQ_ENABLE 4

#define ASM_SYS_CALL_TIMESTAMP  "svc #1"
#define ASM_SYS_CALL_EXC        "svc #2"
#define ASM_SYS_CALL_TIMER      "svc #3"
#define ASM_SYS_CALL_IRQ_ENABLE "svc #4"

#define LAUNCH_SYS_CALL(x)  asm volatile(x)

#endif