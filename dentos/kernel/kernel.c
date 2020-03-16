#include <stdio.h>

#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/tty.h>

// Main method called by boot
void kernel_main() {
	serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_buffer(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);

	printk("%s\n", "Serial setup successfully");

	gdt_init();
	printk("%s\n", "New GDT installed successfully");

	term_init();

	puts("Welcome to the terminal!");
}
