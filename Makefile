NAME := kernel.bin
ISO  := kfs.iso

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := .build

CC := gcc
AS := nasm
LD := ld

CFLAGS   := -m32 -ffreestanding -fno-builtin -fno-stack-protector \
            -fno-pic -fno-pie \
            -Wall -Wextra -Werror -masm=intel
CPPFLAGS := -nostdinc -I$(INC_DIR) -MMD -MP
ASFLAGS  := -f elf32
LDFLAGS  := -m elf_i386 -T linker.ld -nostdlib

SRCS_C := $(shell find $(SRC_DIR) -name '*.c')
SRCS_S := $(shell find $(SRC_DIR) -name '*.asm')
OBJS_C := $(SRCS_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.c.o)
OBJS_S := $(SRCS_S:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.asm.o)
OBJS   := $(OBJS_S) $(OBJS_C)
DEPS   := $(OBJS:.o=.d)

.PHONY: all iso run run-iso clean fclean re

all: $(NAME)

$(NAME): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

iso: $(ISO)

$(ISO): $(NAME) grub.cfg
	mkdir -p iso/boot/grub
	cp $(NAME) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso

run: $(NAME)
	qemu-system-i386 -kernel $(NAME)

run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR) iso

fclean: clean
	rm -f $(NAME) $(ISO)

re: fclean all

-include $(DEPS)
