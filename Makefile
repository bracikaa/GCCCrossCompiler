CC=/opt/cross/bin/i686-elf-gcc
AS=/opt/cross/bin/i686-elf-as
KERNEL_IMG=myos.bin
GRUB = grub.cfg
OBJECTS=kernel.o \
	terminal.o \
	boot.o \
	port.o \
	gdt.o \
	gdt_asm.o \
	#interrupt.o \

CFLAGS= -ggdb -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra


all: $(KERNEL_IMG)

clean:
	-@rm *.o *~


kernel.o : kernel.c terminal.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o 

boot.o : boot.s
	$(AS) --32 boot.s -o boot.o

port.o: port.s
	$(AS) --32 port.s -o port.o

terminal.o : terminal.h terminal.c
	$(CC) $(CFLAGS) -c terminal.c -o terminal.o

$(KERNEL_IMG) : $(OBJECTS) linker.ld
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc

gdt_asm.o : gdt_asm.s
	$(AS) --32 -ggdb gdt_asm.s -o gdt_asm.o

gdt.o : gdt.c gdt.h
	$(CC) $(CFLAGS) -c gdt.c -o gdt.h






