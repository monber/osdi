.global irq_enable
irq_enable:
    msr DAIFClr, 0xf
    ret

.global irq_disable
irq_disable:
    msr DAIFSet, 0xf
    ret
