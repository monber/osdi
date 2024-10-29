SECTIONS
{
    . = 0x80000;
    text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    . = ALIGN(16);
    .bss :
    {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
    __bss_size = (__bss_start - __bss_end) >> 3;
}