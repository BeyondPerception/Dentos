#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "vga.h"

static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH = 80;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000; // Address of VGA buffer after page mapping

static size_t term_row;
static size_t term_col;
static uint8_t term_color;
static uint16_t* term_buffer;

void term_putcharat(unsigned char c, uint8_t color, size_t row, size_t col) {
	const size_t index = row * VGA_WIDTH + col;
	term_buffer[index] = vga_entry(c, color);
}

void term_init(void) {
	term_row = 0;
	term_col = 0;
	term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term_buffer = VGA_MEMORY;
	memset(term_buffer, 0, VGA_HEIGHT * VGA_WIDTH * sizeof(uint16_t));
}

void term_setcolor(uint8_t color) {
	term_color = color;
}

void scroll(int n) {
	if (n >= VGA_HEIGHT) {
		term_init();
	} else {
		memcpy(&term_buffer[0], &term_buffer[n * VGA_WIDTH], VGA_WIDTH * sizeof(uint16_t) * (VGA_HEIGHT - n));
		memset(&term_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH - 1], 0,
			   VGA_WIDTH * sizeof(uint16_t));
	}
}

void term_putc(char c) {
	if (term_row == VGA_HEIGHT) {
		scroll(1);
		term_row--;
		term_col = 0;
	}
	if (c == '\n') {
		term_row++;
		term_col = 0;
	} else {
		term_putcharat(c, term_color, term_row, term_col);
		if (++term_col == VGA_WIDTH) {
			term_col = 0;
			term_row++;
		}
	}
}

void term_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		term_putc(data[i]);
	}
}