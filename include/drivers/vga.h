#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SIZE   (VGA_WIDTH * VGA_HEIGHT)

#define NB_SCREENS 4

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
	size_t row;
	size_t col;
	u8 color;
} t_vga;

/* Global state, defined in vga.c */
extern t_vga  g_screens[NB_SCREENS];
extern size_t g_current_screen;

/* Driver */
void vga_init(void);
void vga_putchar(t_vga *v, char c);
void vga_putstr(t_vga *v, const char *str);

/* Screen management */
// void   vga_switch_screen(size_t index);
t_vga *vga_current(void);

#endif
