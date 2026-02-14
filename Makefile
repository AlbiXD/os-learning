CFLAGS := -m32 -ffreestanding -O2 -fno-stack-protector -fno-pic -fno-pie -nostdlib -nostartfiles
LFLAGS := -m elf_i386 --oformat binary
CC := i686-elf-gcc
LD := i686-elf-ld

all: boot.bin main.bin

main.bin: main.o
	$(LD) $(LFLAGS) -Ttext 0x1000 -e bootmain -o main.bin main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

boot.bin: boot.o
	$(LD) $(LFLAGS) -Ttext 0x7c00 -o boot.bin boot.o

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o
clean:
	rm -f boot.o boot.bin main.o main.bin
output:
	dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
	dd if=main.bin of=boot.img bs=512 seek=1 conv=notrunc
	qemu-system-i386  -drive format=raw,file=boot.img
