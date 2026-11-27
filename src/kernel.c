#include "drivers.h"

void kmain(void)
{
	int key;

	vga_init();
	vga_putstr(vga_current(), "42");

	while (1) {
		key = keyboard_getkey();
		if (!key)
			continue ;
		if (keyboard_macro(key))
			continue ;
		if (key < 0x100)
			vga_putchar(vga_current(), (char)key);
	}
}
