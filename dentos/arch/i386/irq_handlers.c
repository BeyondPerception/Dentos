#include <kernel/io.h>
#include <kernel/kbddriver.h>
#include <stdio.h>

void timer_handler(void) {
	outb(0x20, 0x20);
}

void keyboard_handler(void) {
	unsigned char scan = inb(KEYBOARD_CONTROLLER_PORT);
	handle_keycode(scan);

	outb(0x20, 0x20);
}

void divbyzero_handler(void) {
	puts("Div By Zero Error!");
	outb(0x20, 0x20);
}

void debug_handler(void) {
	puts("Debug Handler Called!");
	outb(0x20, 0x20);
}

void nonmaskable_handler(void) {
	puts("Non-maskable Interrupt!");
	outb(0x20, 0x20);
}

void breakpoint_handler(void) {
	puts("Breakpoint!");
	outb(0x20, 0x20);
}

void overflow_handler(void) {
	puts("Overflow!");
	outb(0x20, 0x20);
}

void boundexceded_handler(void) {
	puts("Bounds Range Exceeded!");
	outb(0x20, 0x20);
}

void invalopcode_handler(void) {
	puts("Invalid Opcode!");
	outb(0x20, 0x20);
}

void devnotavail_handler(void) {
	puts("Device Not Available!");
	outb(0x20, 0x20);
}

void doublefault_handler(int error) {
	printf("Double Fault!: %d\n", error);
	outb(0x20, 0x20);
}

void invalidtss_handler(int error) {
	printf("Invalid TSS!: %d\n", error);
	outb(0x20, 0x20);
}

void seg_notprsnt_handler(int error) {
	printf("Segment Not Present!: %d\n", error);
	outb(0x20, 0x20);
}

void stack_segfault_handler(int error) {
	printf("Stack-Segment Fault!: %d\n", error);
	outb(0x20, 0x20);
}

void gpf_handler(int error) {
	printf("General Protection Fault!: %d\n", error);
	outb(0x20, 0x20);
}

void pagefault_handler(unsigned int addr, unsigned int error) {
	char bitstr[6];
	bitstr[5] = '\0';
	for (int i = 4; i >= 0; i--) {
		bitstr[i] = (error & 1) + '0';
		error >>= 1;
	}
	printf("Page Fault at %p!: %s\n", (void*) addr, bitstr);
	outb(0x20, 0x20);
}

void x87float_exc_handler(void) {
	puts("x87 Floating Point Exception!");
	outb(0x20, 0x20);
}

void align_chk_handler(int error) {
	printf("Align Check!: %d\n", error);
	outb(0x20, 0x20);
}

void machine_chk_handler(void) {
	puts("Machine Check!");
	outb(0x20, 0x20);
}

void SIMDfloat_exc_handler(void) {
	puts("SIMD Floating Point Exception!");
	outb(0x20, 0x20);
}

void virtualization_exc_handler(void) {
	puts("Virtualization Exception!");
	outb(0x20, 0x20);
}

void security_exc_handler(int error) {
	printf("Security Exception!: %d\n", error);
	outb(0x20, 0x20);
}
