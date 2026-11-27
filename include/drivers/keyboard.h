#ifndef KEYBOARD_H
#define KEYBOARD_H

bool keyboard_shift(void);
bool keyboard_caps(void);
bool keyboard_ctrl(void);
bool keyboard_alt(void);

char keyboard_getchar(void);
bool keyboard_macro(char c);

#endif
