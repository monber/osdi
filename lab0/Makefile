all: clean kernel8.img

kernel8.img: lab0.s
	aarch64-linux-gnu-gcc -c lab0.s -o lab0.o
	aarch64-linux-gnu-ld -T linker.ld lab0.o -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -rf lab0.o kernel8.elf

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -display none -d in_asm