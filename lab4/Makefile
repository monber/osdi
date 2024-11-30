#declare variable
CC = aarch64-linux-gnu-gcc
CFLAGS = -c -g -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles
INCLUDE_FLAGS = -Iinclude -Ikernel/include -Ikernel/peripheral/include -Ikernel/exception/include \
                -Ikernel/task/include
SRCS = $(wildcard src/*.c) $(wildcard kernel/*.c) $(wildcard kernel/*/*.c) $(wildcard kernel/*/*/*.c)
ASMS = $(wildcard kernel/*.S) $(wildcard kernel/*/*/*.S) $(wildcard src/*.S)
OBJS = $(SRCS:.c=.o) $(ASMS:.S=.S.o)

all: clean kernel8.img
#all: clean imgloader/kernel8.img

kernel8.img: $(OBJS)
	aarch64-linux-gnu-ld -T link.ld $(OBJS) -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@

%.S.o: %.S
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@ 

clean:
	rm -rf $(OBJS)
	rm -rf kernel8.elf

run:all
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb

gdb:all
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb -s -S
