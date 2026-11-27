#include "lib/string.h"

void *memcpy(void *dest, const void *src, size_t n)
{
	u8 *d;
	const u8 *s;

	d = (u8 *)dest;
	s = (const u8 *)src;

	while (n--)
		*d++ = *s++;

	return (dest);
}
