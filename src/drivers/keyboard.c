#include "drivers/keyboard.h"

#include "drivers/vga.h"
#include "io.h"
#include "lib/ctype.h"
#include "types.h"

#define KBD_DATA   0x60
#define KBD_STATUS 0x64

#define SC_RELEASE 0x80 /* bit 7 set: key released */

#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_CAPS   0x3A
#define SC_CTRL   0x1D
#define SC_ALT    0x38

typedef struct s_kbd_state {
	bool lshift;
	bool rshift;
	bool caps;
	bool ctrl;
	bool alt;
} t_kbd_state;

static t_kbd_state g_kbd_state = {false, false, false, false, false};

static const char keymap[58] = {
	0 /* Nothing */, 0 /* Esc */, '1', '2',
	'3', '4', '5', '6',
	'7', '8', '9', '0',
	'-', '=', '\b', '\t',
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i',
	'o', 'p', '[', ']',
	'\n', 0 /* Ctrl */, 'a', 's',
	'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';',
	'\'', '`', 0 /* LShift */, '\\',
	'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',',
	'.', '/', 0 /* RShift */, '*' /* Keypad */,
	0 /* Alt */, ' '
};

static const char keymap_shift[58] = {
	0 /* Nothing */, 0 /* Esc */, '!', '@',
	'#', '$', '%', '^',
	'&', '*', '(', ')',
	'_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}',
	'\n', 0 /* Ctrl */, 'A', 'S',
	'D', 'F', 'G', 'H',
	'J', 'K', 'L', ':',
	'"', '~', 0 /* LShift */, '|',
	'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<',
	'>', '?', 0 /* RShift */, '*' /* Keypad */,
	0 /* Alt */, ' '
};

bool keyboard_shift(void)
{
	return (g_kbd_state.lshift || g_kbd_state.rshift);
}

bool keyboard_caps(void)
{
	return (g_kbd_state.caps);
}

bool keyboard_ctrl(void)
{
	return (g_kbd_state.ctrl);
}

bool keyboard_alt(void)
{
	return (g_kbd_state.alt);
}

char keyboard_getchar(void)
{
	u8 scancode;
	bool released;
	char c;

	if (!(inb(KBD_STATUS) & 1))
		return (0);

	scancode = inb(KBD_DATA);
	released = scancode & SC_RELEASE;
	scancode &= ~SC_RELEASE;

	switch (scancode) {
	case SC_LSHIFT:
		g_kbd_state.lshift = !released;
		return (0);
	case SC_RSHIFT:
		g_kbd_state.rshift = !released;
		return (0);
	case SC_CAPS:
		if (!released)
			g_kbd_state.caps = !g_kbd_state.caps;
		return (0);
	case SC_CTRL:
		g_kbd_state.ctrl = !released;
		return (0);
	case SC_ALT:
		g_kbd_state.alt = !released;
		return (0);
	default:
		break ;
	}

	if (released || scancode >= sizeof(keymap))
		return (0);

	c = keyboard_shift() ? keymap_shift[scancode] : keymap[scancode];

	if (keyboard_caps()) {
		if (islower(c))
			c = toupper(c);
		else if (isupper(c))
			c = tolower(c);
	}
	return (c);
}

bool keyboard_macro(char c)
{
	if (!keyboard_ctrl())
		return (false);

	if (tolower(c) == 'a') {
		screen_prev();
		return (true);
	}
	if (tolower(c) == 'd') {
		screen_next();
		return (true);
	}
	if (c >= '1' && c <= '1' + NB_SCREENS - 1) {
		screen_switch(c - '1');
		return (true);
	}
	return (false);
}
