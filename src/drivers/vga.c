#include "drivers/vga.h"

#include "lib/ctype.h"
#include "lib/string.h"

#define VGA_MEMORY ((volatile u16 *)0xB8000)

#define VGA_TAB_SIZE 4

t_vga  g_screens[NB_SCREENS];
size_t g_current_screen = 0;

static u16 g_screen_buffers[NB_SCREENS][VGA_SIZE];

static u16 vga_entry(char c, u8 color)
{
	return ((u16)(((u16)color << 8) | (u8)c));
}

static void vga_set_color(t_vga *v, t_vga_color fg, t_vga_color bg)
{
	v->color = (u8)(fg | (bg << 4));
}

static void vga_clear(t_vga *v)
{
	for (size_t i = 0; i < VGA_SIZE; i++)
		v->buffer[i] = vga_entry(' ', v->color);
	v->row = 0;
	v->col = 0;
}

/* The buffer must be set by the caller before calling this */
static void vga_screen_init(t_vga *v)
{
	vga_set_color(v, VGA_WHITE, VGA_BLACK);
	vga_clear(v);
}

void vga_init(void)
{
	memset(g_screens, 0, sizeof(g_screens));
	for (size_t i = 0; i < NB_SCREENS; i++) {
		/* Only the active screen (0) points to the real VGA memory */
		g_screens[i].buffer = (i == 0) ? VGA_MEMORY : g_screen_buffers[i];
		vga_screen_init(&g_screens[i]);
	}
	/* Already init when declaring global */
	g_current_screen = 0;
}

t_vga *vga_current(void)
{
	return (&g_screens[g_current_screen]);
}

// void vga_switch_screen(size_t index)
// {
// 	t_vga *prev;
// 	t_vga *next;

// 	if (index >= NB_SCREENS || index == g_current_screen)
// 		return;
// 	prev = vga_current();
// 	next = &g_screens[index];

// 	/* 1. Save the displayed content into the previous screen's own buffer */
// 	memcpy(g_screen_buffers[g_current_screen], (const void *)prev->buffer,
// 		VGA_SIZE * sizeof(u16));
// 	prev->buffer = g_screen_buffers[g_current_screen];

// 	/* 2. Display the next screen */
// 	memcpy((void *)VGA_MEMORY, (const void *)next->buffer,
// 		VGA_SIZE * sizeof(u16));
// 	next->buffer = VGA_MEMORY;

// 	g_current_screen = index;
// }

void vga_putchar(t_vga *v, char c)
{
	/* Ignore control characters */
	if (!isprint(c))
		return ;

	v->buffer[v->row * VGA_WIDTH + v->col] = vga_entry(c, v->color);
	v->col++;
}

void vga_putstr(t_vga *v, const char *str)
{
	while (*str)
		vga_putchar(v, *str++);
}
