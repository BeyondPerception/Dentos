#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

const int VGA_COLS = 80;
const int VGA_ROWS = 25;

int term_col = 1;
int term_row = 1;
uint8_t term_color = 0x0F;

// Clears the terminal
void term_init() {
	for (int r = 0; r < VGA_ROWS; r++) {
		for (int c = 0; c < VGA_COLS; c++) {
			int index = (VGA_COLS * r) + c;
			vga_buffer[index] = ((uint16_t)term_color<<8) | ' ';
		}
	}
}

void term_putc(char c) {
	if (c == '\n') {
		term_col = 0;
		term_row++;
	} else {
		int index = (VGA_COLS * term_row) + term_col;
		vga_buffer[index] = ((uint16_t)term_color<<8) | c;
		term_col++;
	}

	if (term_col >= VGA_COLS) {
        term_col = 0;
        term_row++;
    }
    if (term_row >= VGA_ROWS) {
    	term_row = 0;
		term_col = 0;
       	// hopefully scroll in the future
    }
}

void term_print(char* str) {
	int i = 0;
	while (str[i] != '\0') {
		term_putc(str[i]);
		i++;
	}
}

// Main method called by start
void kernel_main() {
	term_init();

	term_print("Welcome to the terminal!\n");
}
