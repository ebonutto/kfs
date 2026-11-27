; Multiboot 1 header: GRUB looks for it in the first 8 KB of the kernel
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384              ; 16 KB stack
stack_top:

section .text
global _start
extern kmain

_start:
    mov esp, stack_top      ; C needs a stack
    call kmain
.hang:
    cli
    hlt
    jmp .hang