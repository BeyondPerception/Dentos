#include <stdint.h>
#include <kernel/serial.h>

struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_mid;
	unsigned char access;
	unsigned char granularity; // One byte for limit_high and flags
	unsigned char base_high;
}__attribute__((packed));

struct gdt_entry gdt[3];

void gdt_setentry(int i, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
	// Set base
	gdt[i].base_low = base & 0xFFFF;
	gdt[i].base_mid = (base >> 16) & 0xFF;
	gdt[i].base_high = (base >> 24) & 0xFF;

	// Set limit
	gdt[i].limit_low = limit & 0xFFFF;
	gdt[i].granularity = (limit >> 16) & 0x0F;

	gdt[i].granularity |= gran & 0xF0;
	gdt[i].access = access;
}

/**
 * Clears the gdt and installs the current one
 */
extern void gdt_flush(void);

extern void gdt_set(void* gdt_ptr, short size);

void gdt_init(void) {
	gdt_set(&gdt, sizeof(gdt) - 1);

	gdt_setentry(0, 0, 0, 0, 0);
	// Setup the kernel code segment
	gdt_setentry(1, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);
	// Setup the kernel data segment
	gdt_setentry(2, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);

	gdt_flush();
}
