.section ".text"

#include "syscall_table.h"

.global exec
exec:
    mov     w8,     SYS_CALL_EXC
    svc     #0
    ret

.global fork
fork:
    mov     w8,     SYS_CALL_FORK
    svc     #0
    ret