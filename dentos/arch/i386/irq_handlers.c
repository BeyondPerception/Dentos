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

void doublefault_handler(void) {
	puts("Double Fault!");
	outb(0x20, 0x20);
}

void invalidtss_handler(void) {
	puts("Invalid TSS!");
	outb(0x20, 0x20);
}

void seg_notprsnt_handler(void) {
	puts("Segment Not Present!");
	outb(0x20, 0x20);
}

void stack_segfault_handler(void) {
	puts("Stack-Segment Fault!");
	outb(0x20, 0x20);
}

void gpf_handler(void) {
	puts("General Protection Fault!");
	outb(0x20, 0x20);
}

void pagefault_handler(void) {
	puts("Page Fault!");
	outb(0x20, 0x20);
}

void x87float_exc_handler(void) {
	puts("x87 Floating Point Exception!");
	outb(0x20, 0x20);
}

void align_chk_handler(void) {
	puts("Align Check!");
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

void security_exc_handler(void) {
	puts("Security Exception!");
	outb(0x20, 0x20);
}