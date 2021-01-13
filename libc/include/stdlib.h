#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__)) void abort(void);
void* malloc(unsigned int);
void free(unsigned int);

#ifdef __cplusplus
}
#endif

#endif
