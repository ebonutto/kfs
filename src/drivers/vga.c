#include "drivers/vga.h"

#include "lib/ctype.h"
#include "lib/string.h"

#define VGA_MEMORY ((volatile u16 *)0xB8000)

#define VGA_TAB_SIZE 4

static t_screen g_screens[NB_SCREENS];
static size_t   g_current_screen = 0;

/* Saved content of the screens that are not displayed */
static u16 g_backup[NB_SCREENS][VGA_SIZE];

/* Hardware level: VGA cell format and video memory */

static u16 vga_entry(char c, u8 color)
{
	return ((u16)(((u16)color << 8) | (u8)c));
}

static void vga_copy(volatile u16 *dest, volatile u16 *src, size_t n)
{
	for (size_t i = 0; i < n; i++)
		dest[i] = src[i];
}

/* Virtual screen level */

static void screen_set_color(t_screen *s, t_vga_color fg, t_vga_color bg)
{
	s->color = (u8)(fg | (bg << 4));
}

static void screen_clear(t_screen *s)
{
	for (size_t i = 0; i < VGA_SIZE; i++)
		s->buffer[i] = vga_entry(' ', s->color);
	s->row = 0;
	s->col = 0;
}

/* The buffer must be set by the caller before calling this */
static void screen_init(t_screen *s)
{
	screen_set_color(s, VGA_WHITE, VGA_BLACK);
	screen_clear(s);
}

void vga_init(void)
{
	memset(g_screens, 0, sizeof(g_screens));
	for (size_t i = 0; i < NB_SCREENS; i++) {
		/* Only the active screen (0) points to the real VGA memory */
		g_screens[i].buffer = (i == 0) ? VGA_MEMORY : g_backup[i];
		screen_init(&g_screens[i]);
	}
	/* Reset in case vga_init is called twice */
	g_current_screen = 0;
}

/* Move every line up by one and clear the last line */
static void screen_scroll(t_screen *s)
{
	vga_copy(s->buffer, s->buffer + VGA_WIDTH, VGA_SIZE - VGA_WIDTH);
	for (size_t i = VGA_SIZE - VGA_WIDTH; i < VGA_SIZE; i++) /* vga_set() */
		s->buffer[i] = vga_entry(' ', s->color);
	s->row = VGA_HEIGHT - 1;
}

static void screen_newline(t_screen *s)
{
	s->col = 0;
	s->row++;
	if (s->row >= VGA_HEIGHT)
		screen_scroll(s);
}

static void screen_backspace(t_screen *s)
{
	if (s->col > 0) {
		s->col--;
	} else if (s->row > 0) {
		s->row--;
		s->col = VGA_WIDTH - 1;
	} else {
		return ;
	}
	s->buffer[s->row * VGA_WIDTH + s->col] = vga_entry(' ', s->color);
}

void screen_putchar(t_screen *s, char c)
{
	size_t n;

	if (c == '\n') {
		screen_newline(s);
		return ;
	}
	if (c == '\b') {
		screen_backspace(s);
		return ;
	}
	if (c == '\t') {
		/* Pad with spaces up to the next tab stop */
		n = VGA_TAB_SIZE - (s->col % VGA_TAB_SIZE);
		while (n--)
			screen_putchar(s, ' ');
		return ;
	}
	/* Ignore other control characters */
	if (!isprint(c))
		return ;

	s->buffer[s->row * VGA_WIDTH + s->col] = vga_entry(c, s->color);
	s->col++;
	if (s->col >= VGA_WIDTH)
		screen_newline(s);
}

void screen_putstr(t_screen *s, const char *str)
{
	while (*str)
		screen_putchar(s, *str++);
}

t_screen *screen_current(void)
{
	return (&g_screens[g_current_screen]);
}

void screen_switch(size_t index)
{
	t_screen *old;
	t_screen *new;

	if (index >= NB_SCREENS || index == g_current_screen)
		return ;

	old = &g_screens[g_current_screen];
	new = &g_screens[index];

	/* Save what is displayed into the old screen's backup */
	vga_copy(g_backup[g_current_screen], VGA_MEMORY, VGA_SIZE);
	old->buffer = g_backup[g_current_screen];

	/* Display the new screen */
	vga_copy(VGA_MEMORY, new->buffer, VGA_SIZE);
	new->buffer = VGA_MEMORY;

	g_current_screen = index;
}

void screen_prev(void)
{
	screen_switch((g_current_screen + NB_SCREENS - 1) % NB_SCREENS);
}

void screen_next(void)
{
	screen_switch((g_current_screen + 1) % NB_SCREENS);
}
