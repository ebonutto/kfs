#include "vga.h"

#define VGA_MEMORY ((volatile u16 *)0xB8000)

static u16 vga_cell(char c, u8 color)
{
	return ((u16)((color << 8) | (u8)c));
}

void vga_set_color(t_vga *v, t_vga_color fg, t_vga_color bg)
{
	v->color = (u8)(fg | (bg << 4));
}

void vga_clear(t_vga *v)
{
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		v->buffer[i] = vga_cell(' ', v->color);
	v->row = 0;
	v->col = 0;
}

void vga_init(t_vga *v)
{
	v->buffer = VGA_MEMORY;
	vga_set_color(v, VGA_WHITE, VGA_BLACK);
	vga_clear(v);
}

void vga_putchar(t_vga *v, char c)
{
	if (c == '\n') {
		v->col = 0;
		v->row++;
	} else {
		v->buffer[v->row * VGA_WIDTH + v->col] = vga_cell(c, v->color);
		v->col++;
	}

	if (v->col >= VGA_WIDTH) {
		v->col = 0;
		v->row++;
	}

	if (v->row >= VGA_HEIGHT)
		v->row = 0;
}

void vga_putstr(t_vga *v, const char *str)
{
	while (*str)
		vga_putchar(v, *str++);
}
