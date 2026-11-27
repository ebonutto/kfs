#include "lib/string.h"

void *memset(void *s, int c, size_t n)
{
	u8 *p;

	p = (u8 *)s;

	while (n--)
		*p++ = (u8)c;

	return (s);
}
