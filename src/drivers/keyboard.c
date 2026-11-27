#include "drivers/keyboard.h"

#include "io.h"
#include "types.h"

#define KBD_DATA 0x60
#define KBD_STATUS 0x64

#define KBD_RELEASE 0x80 // KEY_RELEASE

#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_CTRL 0x1D
#define SC_ALT 0x38
#define SC_CAPS 0x3A

typedef struct s_kbd_state {
	bool shift;
	bool ctrl;
	bool alt;
	bool caps;
} t_kbd_state;

static t_kbd_state g_state = {false, false, false, false};

static const char keymap[58] = {
	0 /* Nothing */, 0 /* Esc */, '1', '2',
	'3', '4', '5', '6',
	'7', '8', '9', '0',
	'-', '=', '\b', '\t',
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i',
	'o', 'p', '[', ']',
	'\n', 0 /* LCtrl */, 'a', 's',
	'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';',
	'\'', '`', 0 /* LShift */, '\\',
	'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',',
	'.', '/', 0 /* RShift */, '*' /* Keypad */,
	0 /* LAlt */, ' '
};

static const char keymap_shift[58] = {
	0 /* Nothing */, 0 /* Esc */, '!', '@',
	'#', '$', '%', '^',
	'&', '*', '(', ')',
	'_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}',
	'\n', 0 /* LCtrl */, 'A', 'S',
	'D', 'F', 'G', 'H',
	'J', 'K', 'L', ':',
	'"', '~', 0 /* LShift */, '|',
	'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<',
	'>', '?', 0 /* RShift */, '*' /* Keypad */,
	0 /* LAlt */, ' '
};

bool keyboard_ctrl(void)
{
	return (g_state.ctrl);
}

bool keyboard_alt(void)
{
	return (g_state.alt);
}

bool keyboard_shift(void)
{
	return (g_state.shift);
}

char keyboard_getchar(void)
{
	u8 scancode;
	bool released;
	char c;

	if (!(inb(KBD_STATUS) & 1))
		return (0);

	scancode = inb(KBD_DATA);
	released = scancode & KBD_RELEASE;
	scancode &= ~KBD_RELEASE;

	if (scancode == SC_LSHIFT || scancode == SC_RSHIFT) {
		state.shift = !released;
		return (0);
	}
	if (scancode == SC_CTRL) {
		state.ctrl = !released;
		return (0);
	}
	if (scancode == SC_ALT) {
		state.alt = !released;
		return (0);
	}
	if (scancode == SC_CAPS) {
		if (!released)
			state.caps = !state.caps;
		return (0);
	}

	if (released || scancode >= sizeof(keymap))
		return (0);

	c = state.shift ? keymap_shift[scancode] : keymap[scancode];

	if (state.caps) {
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';   /* to uppercase */
		else if (c >= 'A' && c <= 'Z')
			c += 'a' - 'A';   /* to lowercase (shift + caps) */
	}
	return (c);
}

bool keyboard_macro(char c)
{
	if (!keyboard_ctrl())
		return (false);

	if (key == 'a' || key == 'A') {
		screen_prev();
		return (true);
	}
	if (key == 'd' || key == 'D') {
		screen_next();
		return (true);
	}
	if (key >= '1' && key <= '4') {
		screen_switch(key - '1');
		return (true);
	}
	return (false);
}
