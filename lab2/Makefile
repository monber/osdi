#declare variable
CC = aarch64-linux-gnu-gcc
CFLAGS = -c -Wall -Iinclude -ffreestanding -nostdinc -nostdlib -nostartfiles
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
K_SRCS = $(wildcard kernel/*.c)
K_OBJS = $(K_SRCS:.c=.o) \
			kernel/start.o
L_SRCS = imgloader/main.c src/uart.c src/string.c src/mbox.c
L_OBJS = $(L_SRCS:.c=.o) \
			imgloader/start.o

all: clean kernel/kernel8.img imgloader/imgloader.img clean
#all: clean imgloader/kernel8.img

kernel/kernel8.img: $(OBJS) $(K_OBJS)
	aarch64-linux-gnu-ld -T kernel/link.ld $(OBJS) $(K_OBJS) -o kernel/kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel/kernel8.elf kernel/kernel8.img

imgloader/imgloader.img: $(L_OBJS)
	aarch64-linux-gnu-ld -T imgloader/link.ld $(L_OBJS) -o imgloader/imgloader.elf
	aarch64-linux-gnu-objcopy -O binary imgloader/imgloader.elf imgloader/imgloader.img


%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf kernel/*.o
	rm -rf src/*.o
	rm -rf imgloader/*.o
	rm -rf kernel/kernel8.elf
	rm -rf imgloader/imgloader.elf

run:all
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial stdio -initrd archive/initramfs.cpio -dtb kernel/bcm2710-rpi-3-b-plus.dtb

load:all
	qemu-system-aarch64 -M raspi3 -kernel imgloader/imgloader.img -serial pty

loadgdb:all
	    qemu-system-aarch64 -M raspi3 -kernel imgloader/imgloader.img -serial pty -S -s
