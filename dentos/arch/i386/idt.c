#include <kernel/serial.h>
#include <stdbool.h>
#include <kernel/io.h>

#define INTERRUPT_GATE 0xE
#define TRAP_GATE 0xF
#define TASK_GATE 0x5

#define PIC1            0x20 /* IO base address for master PIC */
#define PIC2            0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 1)

#define ICW1_INIT       0x10   /* Initialization - required! */
#define ICW1_ICW4       0x01   /* ICW4 (not) needed */
#define ICW4_8086       0x01   /* 8086/88 (MCS-80/85) mode */

struct idt_gate {
	unsigned short offset_low;  // offset bits 0..15
	unsigned short selector;    // a code segment selector in GDT or LDT
	unsigned char zero;         // unused, set to 0
	unsigned char type_attr;    // type and attributes
	unsigned short offset_high; // offset bits 16..31
} __attribute__((packed));

struct idt_gate idt[256];

extern void idt_set(void* idt_ptr, short size);

void PIC_remap(int offset1, int offset2) {
	unsigned char a1, a2;

	a1 = inb(PIC1_DATA); // save masks
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the initialization sequence (in cascade mode)
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, offset1); // ICW2: Parent PIC vector offset
	outb(PIC2_DATA, offset2); // ICW2: Child PIC vector offset

	outb(PIC1_DATA, 4); // ICW3: tell Parent PIC that there is a Child PIC at IRQ2
	outb(PIC2_DATA, 2); // ICW3: tell Child PIC its cascade identity

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	outb(PIC1_DATA, a1); // restore saved masks
	outb(PIC2_DATA, a2);
}

void add_irq(unsigned char i, unsigned int offset, unsigned short selector, unsigned char type_attr) {
	idt[i].offset_low = offset & 0xFFFF;
	idt[i].selector = selector;
	idt[i].zero = 0;
	idt[i].type_attr = type_attr;
	idt[i].offset_high = offset >> 16 & 0xFFFF;
}

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq16(void);
extern void irq17(void);
extern void irq18(void);
extern void irq19(void);
extern void irq20(void);
extern void irq30(void);

extern void irq32(void);
extern void irq33(void);

void idt_init(void) {
	PIC_remap(0x20, 0x28);

	printk("IDT Location: %p\n", idt);

	// 0x8 is the kernel code segment. Each gdt entry is 8 bytes long, and the kernel code segment is the second one.
	add_irq(0, (unsigned long) irq0, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(1, (unsigned long) irq1, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(2, (unsigned long) irq2, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(3, (unsigned long) irq3, 0x8, 0b1000 << 4 | TRAP_GATE);
	add_irq(4, (unsigned long) irq4, 0x8, 0b1000 << 4 | TRAP_GATE);
	add_irq(5, (unsigned long) irq5, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(6, (unsigned long) irq6, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(7, (unsigned long) irq7, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(8, (unsigned long) irq8, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(10, (unsigned long) irq10, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(11, (unsigned long) irq11, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(12, (unsigned long) irq12, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(13, (unsigned long) irq13, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(14, (unsigned long) irq14, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(16, (unsigned long) irq16, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(17, (unsigned long) irq17, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(18, (unsigned long) irq18, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(19, (unsigned long) irq19, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(20, (unsigned long) irq20, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(30, (unsigned long) irq30, 0x8, 0b1000 << 4 | INTERRUPT_GATE);


	add_irq(32, (unsigned long) irq32, 0x8, 0b1000 << 4 | INTERRUPT_GATE);
	add_irq(33, (unsigned long) irq33, 0x8, 0b1000 << 4 | INTERRUPT_GATE);

	idt_set(&idt, sizeof(idt) - 1);
}