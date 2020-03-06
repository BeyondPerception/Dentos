#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif

// Main method called by start
void kernel_main() {
	term_init();

	term_print("Welcome to the terminal!\n");
}
