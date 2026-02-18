CFLAGS := -m32 -ffreestanding -O2 -fno-stack-protector -fno-pic -fno-pie -nostdlib -nostartfiles
LFLAGS := -m elf_i386 --oformat binary
CC := i686-elf-gcc
LD := i686-elf-ld

KERNEL_FLAGS := -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -nostdlib -nostartfiles -c



BOOTDIR := boot
BUILDDIR := build


$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	
	
all: $(BUILDDIR) boot.bin main.bin $(BUILDDIR)/kernel.out

$(BUILDDIR)/kernel.o: kernel/kernel.c | $(BUILDDIR)
	$(CC) $(KERNEL_FLAGS) $< -o $@

$(BUILDDIR)/kernel.out: $(BUILDDIR)/kernel.o
	$(LD) -T linker-scripts/kernel.ld $< -o $@
	
main.bin: $(BUILDDIR)/bootmain.o
	$(LD) $(LFLAGS) -Ttext 0x1000 -e bootmain -o $(BUILDDIR)/$@ $^

$(BUILDDIR)/bootmain.o: $(BOOTDIR)/bootmain.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

boot.bin: $(BUILDDIR)/boot.o
	$(LD) $(LFLAGS) -Ttext 0x7c00 -o $(BUILDDIR)/$@ $<

$(BUILDDIR)/boot.o: $(BOOTDIR)/boot.S | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) boot.bin main.bin boot.img

output: all
	rm -f boot.img
	dd if=$(BUILDDIR)/boot.bin of=boot.img bs=512 count=1 conv=notrunc
	dd if=$(BUILDDIR)/main.bin of=boot.img bs=512 seek=1 conv=notrunc
	dd if=$(BUILDDIR)/kernel.out of=boot.img bs=512 seek=2 conv=notrunc
	qemu-system-i386 -drive format=raw,file=boot.img
