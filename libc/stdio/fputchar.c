#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#include <kernel/serial.h>
#endif

int fputchar(enum OUTSTREAM stream, int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	if (stream == STDOUT) {
		term_write(&c, sizeof(c));
	} else if (stream == SERIAL) {
		serial_write(SERIAL_COM1_BASE, &c, sizeof(c));
	}
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
