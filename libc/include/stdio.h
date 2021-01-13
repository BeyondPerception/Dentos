#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

enum OUTSTREAM {
	STDOUT,
	SERIAL,
};

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
int fprintf(enum OUTSTREAM, const char* __restrict, ...);
int vfprintf(enum OUTSTREAM, const char* __restrict, va_list);
int fputchar(enum OUTSTREAM, int);

#ifdef __cplusplus
}
#endif

#endif
