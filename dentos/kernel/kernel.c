#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kernel/tty.h>

// Main method called by boot
void kernel_main() {
	term_init();

	puts("Welcome to the terminal!");
}
