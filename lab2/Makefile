#declare variable
CC = aarch64-linux-gnu-gcc
CFLAGS = -c -Wall -Iinclude -ffreestanding -nostdinc -nostdlib -nostartfiles
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
K_SRCS = $(wildcard kernel/*.c)
K_OBJS = $(K_SRCS:.c=.o) \
			kernel/start.o

all: clean kernel/kernel8.img clean

kernel/kernel8.img: $(OBJS) $(K_OBJS)
	aarch64-linux-gnu-ld -T kernel/link.ld $(OBJS) $(K_OBJS) -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel/kernel8.img

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf kernel/*.o
	rm -rf src/*.o
	rm -rf kernel8.elf

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel/kernel8.img -serial null -serial stdio -initrd archive/initramfs.cpio