#include <stdarg.h>
#include <stdio.h>

int printf(const char* restrict format, ...) {
	va_list args;
	va_start(args, format);
	int r = vfprintf(STDOUT, format, args);
	va_end(args);
	return r;
}
