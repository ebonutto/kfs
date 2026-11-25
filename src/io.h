#ifndef IO_H
#define IO_H

#include "types.h"

/* Read one byte from an I/O port */
static inline u8 inb(u16 port)
{
	u8 value;

	__asm__ __volatile__ (
		  "in al, dx"
		: "=a" (value)
		: "Nd" (port)
	);
	return (value);
}

#endif
