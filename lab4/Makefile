#declare variable
CC = aarch64-linux-gnu-gcc
CFLAGS = -c -g -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles
INCLUDE_FLAGS = -Iinclude -Iperipheral/include -Ikernel -Ikernel/include
SRCS = $(wildcard src/*.c) $(wildcard peripheral/*/*.c) $(wildcard kernel/*.c) \
       $(wildcard kernel/*/*.c)
ASMS = $(wildcard kernel/*.S) $(wildcard kernel/*/*.S)
OBJS = $(SRCS:.c=.o)

OBJS = $(SRCS:.c=.o) $(ASMS:.S=.o)

all: clean kernel/kernel8.img
#all: clean imgloader/kernel8.img

kernel/kernel8.img: $(OBJS)
	aarch64-linux-gnu-ld -T kernel/link.ld $(OBJS) -o kernel/kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel/kernel8.elf kernel/kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@ 

clean:
	rm -rf kernel/*.o kernel/*/*.o
	rm -rf src/*.o
	rm -rf peripheral/src/*.o
	rm -rf kernel/kernel8.elf

run:all
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb

gdb:all
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb -s -S
