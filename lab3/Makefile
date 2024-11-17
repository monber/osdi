#declare variable
CC = aarch64-linux-gnu-gcc
CFLAGS = -c -g -Wall -Iinclude -ffreestanding -nostdinc -nostdlib -nostartfiles
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
K_SRCS = $(wildcard kernel/*.c)
K_ASMS = $(wildcard kernel/*.S)
K_OBJS = $(K_SRCS:.c=.o) \
		 $(K_ASMS:.S=.o)

all: clean kernel/kernel8.img
#all: clean imgloader/kernel8.img

kernel/kernel8.img: $(OBJS) $(K_OBJS)
	aarch64-linux-gnu-ld -T kernel/link.ld $(OBJS) $(K_OBJS) -o kernel/kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel/kernel8.elf kernel/kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf kernel/*.o
	rm -rf src/*.o
	rm -rf kernel/kernel8.elf

run:all
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb

gdb:all
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb -s -S
