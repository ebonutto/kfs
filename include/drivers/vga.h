#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SIZE   (VGA_WIDTH * VGA_HEIGHT)

#define NB_SCREENS 4

typedef enum e_vga_color {
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

typedef struct s_screen {
	volatile u16 *buffer;
	size_t row;
	size_t col;
	u8 color;
} t_screen;

/* Driver */
void vga_init(void);

/* Virtual screens */
void      screen_putchar(t_screen *s, char c);
void      screen_putstr(t_screen *s, const char *str);
t_screen *screen_current(void);
void      screen_switch(size_t index);
void      screen_prev(void);
void      screen_next(void);

#endif
