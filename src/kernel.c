#include "keyboard.h"
#include "vga.h"

void kmain(void)
{
	t_vga screen;
	char c;

	vga_init(&screen);
	vga_putstr(&screen, "42");

	while (1) {
		c = keyboard_getchar();
		if (c)
			vga_putchar(&screen, c);
	}
}
