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

.global exit
exit:
    mov     w8,     SYS_CALL_EXIT
    svc     #0
    ret

.global get_task_id
get_task_id:
    mov     w8,     SYS_CALL_GET_TASK_ID
    svc     #0
    ret