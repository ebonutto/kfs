#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef enum s_vga_color {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GREY,
	VGA_DARK_GREY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_MAGENTA,
	VGA_YELLOW,
	VGA_WHITE
} t_vga_color;

typedef struct s_vga {
	volatile u16 *buffer;
	size_t row; // u8 ?
	size_t col; // u8 ?
	u8 color;
} t_vga;

void vga_init(t_vga *v);
void vga_clear(t_vga *v);
void vga_set_color(t_vga *v, t_vga_color fg, t_vga_color bg);
void vga_putchar(t_vga *v, char c);
void vga_putstr(t_vga *vga, const char *str);

#endif
