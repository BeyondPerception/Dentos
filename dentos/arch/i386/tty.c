#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH = 80;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000;

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
    for (size_t r = 0; r < VGA_HEIGHT; r++) {
        for (size_t c = 0; c < VGA_WIDTH; c++) {
            term_putcharat(' ', term_color, r, c);
        }
    }
}

void term_setcolor(uint8_t color) {
    term_color = color;
}

void scroll(int n) {
    for (int i = 0; i < n; i++) {
        for (size_t j = 0; j < VGA_HEIGHT - 1; j++) {
            memcpy(&term_buffer[j * VGA_WIDTH], &term_buffer[(j + 1) * VGA_WIDTH], VGA_WIDTH * sizeof(int));
        }
        memset(&term_buffer[(VGA_WIDTH - 1) * VGA_HEIGHT - 1], 0, VGA_WIDTH * sizeof(int));
    }
}

void term_putc(char c) {
    unsigned char uc = c;
    if (uc == '\n') {
        term_row++;
        term_col = 0;
    } else {
        term_putcharat(uc, term_color, term_row, term_col);
    }
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT)
            scroll(1);
    }
}

void term_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        term_putc(data[i]);
    }
}

void term_writestring(const char* data) {
    term_write(data, strlen(data));
}
