// #include "printk.h"

// #include "vga.h"

// #include <stdarg.h>

// int vprintk(const char *fmt, va_list args)
// {
// 	for (; *fmt, fmt++) {
// 		if (*fmt != '%') {
// 			vga_putchar(*fmt);
// 			continue;
// 		}
// 		fmt++;
// 		switch(*fmt) {
// 		case 'c':
// 			vga_putchar((char)va_arg(args, int));
// 		}
// 	}
// }

// int printk(const char *fmt, ...)
// {
// 	va_list args;
// 	int r;

// 	va_start(args, fmt);
// 	r = vprintk(fmt, args);
// 	va_end(args);

// 	return (r);
// }
