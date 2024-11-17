.section   .text

.macro exc_save_reg
    sub sp, sp, #8*31
    str x0, [sp]
    str x1, [sp, #8]
    str x2, [sp, #8*2]
    str x3, [sp, #8*3]
    str x4, [sp, #8*4]
    str x5, [sp, #8*5]
    str x6, [sp, #8*6]
    str x7, [sp, #8*7]
    str x8, [sp, #8*8]
    str x9, [sp, #8*9]
    str x10, [sp, #8*10]
    str x11, [sp, #8*11]
    str x12, [sp, #8*12]
    str x13, [sp, #8*13]
    str x14, [sp, #8*14]
    str x15, [sp, #8*15]
    str x16, [sp, #8*16]
    str x17, [sp, #8*17]
    str x18, [sp, #8*18]
    str x19, [sp, #8*19]
    str x20, [sp, #8*20]
    str x21, [sp, #8*21]
    str x22, [sp, #8*22]
    str x23, [sp, #8*23]
    str x24, [sp, #8*24]
    str x25, [sp, #8*25]
    str x26, [sp, #8*26]
    str x27, [sp, #8*27]
    str x28, [sp, #8*28]
    str x29, [sp, #8*29]
    str x30, [sp, #8*30]
.endm

.macro exc_load_reg
    ldr x0, [sp]
    ldr x1, [sp, #8]
    ldr x2, [sp, #8*2]
    ldr x3, [sp, #8*3]
    ldr x4, [sp, #8*4]
    ldr x5, [sp, #8*5]
    ldr x6, [sp, #8*6]
    ldr x7, [sp, #8*7]
    ldr x8, [sp, #8*8]
    ldr x9, [sp, #8*9]
    ldr x10, [sp, #8*10]
    ldr x11, [sp, #8*11]
    ldr x12, [sp, #8*12]
    ldr x13, [sp, #8*13]
    ldr x14, [sp, #8*14]
    ldr x15, [sp, #8*15]
    ldr x16, [sp, #8*16]
    ldr x17, [sp, #8*17]
    ldr x18, [sp, #8*18]
    ldr x19, [sp, #8*19]
    ldr x20, [sp, #8*20]
    ldr x21, [sp, #8*21]
    ldr x22, [sp, #8*22]
    ldr x23, [sp, #8*23]
    ldr x24, [sp, #8*24]
    ldr x25, [sp, #8*25]
    ldr x26, [sp, #8*26]
    ldr x27, [sp, #8*27]
    ldr x28, [sp, #8*28]
    ldr x29, [sp, #8*29]
    ldr x30, [sp, #8*30]
    add sp, sp, #8*31
.endm

exc_hlr:
    exc_save_reg
    bl exc_print_info
    exc_load_reg
    eret

invalid_entry_hlr:
    exc_save_reg
    bl  exc_invalid_entry
    exc_load_reg
    eret

.macro  ventry  label
    b   \label // branch to a handler function.
    .align  7  // entry size is 0x80, .align will pad 0
.endm

// Simple vector table
.align 11 // vector table should be aligned to 0x800
.global exception_table
exception_table:
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry exc_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr

    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr
    ventry invalid_entry_hlr