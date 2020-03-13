#include <string.h>

/**
 * A wider version of memset that uses 2 bytes instead of one
 */
void* memsetw(void* dest, register int val, register size_t len) {
	register unsigned short* ptr = (unsigned short*) dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}