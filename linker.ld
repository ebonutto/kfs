/* The bootloader will start execution at this symbol (defined in boot.s). */
ENTRY(_start)

SECTIONS
{
	/* Load the kernel at 1 MiB, above the BIOS and VGA memory. */
	. = 1M;
	kernel_start = .;

	/* Code. The multiboot header must come first so GRUB finds it
	within the first 8 KiB of the image. */
	.text : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data (constant strings, etc.). */
	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Initialized global variables. */
	.data : ALIGN(4K)
	{
		*(.data)
	}

	/* Uninitialized global variables and the stack. */
	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	/* End of the kernel image. */
	kernel_end = .;
}
