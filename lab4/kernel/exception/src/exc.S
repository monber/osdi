.section   .text

.macro kernel_entry
    sub sp, sp, 32 * 9
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]

    mrs x0, elr_el1
    stp x30, x0, [sp, 16 * 15]
    mrs x0, spsr_el1
    mrs x1, SP_EL0
    stp x0, x1, [sp ,16 * 16]
.endm

.macro kernel_exit
    ldp x30, x0, [sp ,16 * 15]
    msr elr_el1, x0
    ldp x0, x1, [sp ,16 * 16]
    msr spsr_el1, x0
    msr SP_EL0, x1

    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    add sp, sp, 32 * 9
    eret
.endm

exc_hlr_EL1h:
    kernel_entry
    bl exc_entry
    kernel_exit

irq_hlr_EL1h:
    kernel_entry
    bl irq_entry
    kernel_exit

exc_hlr_EL064:
    kernel_entry
    bl exc_entry
    bl task_resched
    kernel_exit

irq_hlr_EL064:
    kernel_entry
    bl irq_entry
    bl task_resched
    kernel_exit

invalid_entry_hlr:
    kernel_entry
    bl  exc_invalid_entry
    kernel_exit

.macro  ventry  label
    b   \label // branch to a handler function.
    .align  7  // entry size is 0x80, .align will pad 0
.endm

.global from_EL2_to_EL1
from_EL2_to_EL1:
    //set HCR_EL2.RW to 1 for running on aarch64
    mov x0, (1 << 31)
    msr HCR_EL2, x0 
    //set interrupt mask, EL and sp
    mov x0, 0x5
    msr spsr_el2, x0
    //set return address
    msr elr_el2, lr
    //set EL1 sp
    mov x0, 0x70000
    msr SP_EL1, x0
    eret

.global from_EL1_to_EL0
from_EL1_to_EL0:
    //set CPACR_EL1.FPEN for floating point problem
    mrs x0, CPACR_EL1
    orr x0, x0, #(3 << 20)
    msr CPACR_EL1, x0
    //set interrupt mask, EL and sp
    mov x0, 0x0
    msr spsr_el1, x0
    //set return address
    msr elr_el1, lr
    //set EL0 sp
    mov x0, 0x60000
    msr SP_EL0, x0
    eret

// Simple vector table
.align 11 // vector table should be aligned to 0x800
.global exception_table
exception_table:
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry exc_hlr_EL1h
    ventry irq_hlr_EL1h
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry exc_hlr_EL064
    ventry irq_hlr_EL064
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

.globl task_ret_from_fork
task_ret_from_fork:
    mrs x2, tpidr_el1
    ldp x1, x0, [x2]
    cbz x1, task_ret_to_user
    blr x1

task_ret_to_user:
    kernel_exit

