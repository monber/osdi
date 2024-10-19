SECTIONS
{
  . = 0x80000; //default address for RPi3 ARMv8
  .text : { *(.text) }
}