#include "vga.h"

void kmain(void)
{
	t_vga screen;

	vga_init(&screen);
	vga_putstr(&screen, "42");

	while (1);
		// c = keyboard_getchar();
		// if (c)
		// 	vga_putchar(&screen, c);
}
