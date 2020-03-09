#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

// Main method called by boot
void kernel_main() {
	term_init();

	term_print("Welcome to the terminal!\n");
}
