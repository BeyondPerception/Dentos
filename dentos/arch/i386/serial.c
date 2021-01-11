#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include <kernel/io.h>

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/**
 * Sets the baud rate for the specified serial port to (115200 / divisor) bits/s.
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), divisor >> 8); // First 8 bits
	outb(SERIAL_DATA_PORT(com), divisor & 0xFF); // Last 8 bits
}

// These use some magic numbers to configure the port properly.
// More info on these values can be found here: https://en.wikibooks.org/wiki/Serial_Programming

void serial_configure_line(unsigned short com) {
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffer(unsigned short com) {
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com) {
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(unsigned int com) {
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(unsigned short com, const char* data, size_t len) {
	for (size_t i = 0; i < len; i++) {
		while (!serial_is_transmit_fifo_empty(com)) { /* block */ }
		outb(SERIAL_DATA_PORT(com), data[i]);
	}
}

int printk(const char* restrict format, ...) {
	va_list args;
	va_start(args, format);
	int r = vfprintf(SERIAL, format, args);
	va_end(args);
	return r;
}

void putsk(const char* string) {
	printk("%s\n", string);
}
