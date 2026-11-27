#include "drivers.h"

void kmain(void)
{
	char c;

	vga_init();
	screen_putstr(screen_current(), "Hello 42 !\n");

	while (1) {
		c = keyboard_getchar();
		if (!c)
			continue ;
		if (keyboard_macro(c))
			continue ;
		if (keyboard_ctrl() || keyboard_alt())
			continue ;
		screen_putchar(screen_current(), c);
	}
}
