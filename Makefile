# Tools
CC      = gcc
AS      = nasm
LD      = ld

# 32-bit freestanding build (no libc, no host headers)
CFLAGS  = -m32 -ffreestanding -fno-builtin -fno-stack-protector \
          -nostdlib -nodefaultlibs -Wall -Wextra -Werror
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Files
NAME    = kernel.bin
ISO     = kfs.iso
SRCS_C = $(wildcard src/*.c)
SRCS_S = $(wildcard src/*.asm)
OBJS   = $(SRCS_C:.c=.o) $(SRCS_S:.asm=.o)

all: $(NAME)

$(NAME): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

# Bootable ISO (needs grub-mkrescue, xorriso, mtools)
iso: $(ISO)

$(ISO): $(NAME) grub.cfg
	mkdir -p iso/boot/grub
	cp $(NAME) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

# Quick test: QEMU loads the multiboot kernel directly (no GRUB, no ISO)
run: $(NAME)
	qemu-system-i386 -kernel $(NAME)

# Test the real GRUB boot path
run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(OBJS)
	rm -rf iso

fclean: clean
	rm -f $(NAME) $(ISO)

re: fclean all

.PHONY: all iso run run-iso clean fclean re