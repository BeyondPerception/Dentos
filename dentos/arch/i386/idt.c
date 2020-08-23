#include <stdbool.h>
#include <kernel/serial.h>

#define INTERRUPT_GATE 0xE
#define TRAP_GATE 0xF
#define TASK_GATE 0x5

struct idt_gate {
	unsigned short offset_low; // offset bits 0..15
	unsigned short selector; // a code segment selector in GDT or LDT
	unsigned char zero;      // unused, set to 0
	unsigned char type_attr; // type and attributes
	unsigned short offset_high; // offset bits 16..31
}__attribute__((packed));

struct idt_gate idt[256];

void idt_setentry(int i, unsigned int offset, bool present, unsigned char dpl, bool segment, unsigned char type) {
	idt[i].offset_low = offset & 0xFFFF;
	idt[i].offset_high = offset >> 16 & 0xFFFF;
	idt[i].type_attr = present << 8 | dpl << 7 | segment << 5 | type;
}

extern void idt_set(void* idt_ptr, short size);

void idt_init(void) {
	idt_set(&idt, sizeof(idt) - 1);
}