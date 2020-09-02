#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/tty.h>
#include <kernel/idt.h>

extern void _kernel_start(void);

extern void _kernel_end(void);

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low, base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low, length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;

// this is really an entry, not the entire map.
typedef multiboot_memory_map_t mmap_entry_t;

// Main method called by boot
_Noreturn void kernel_main(multiboot_info_t* mbd) {
	serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_buffer(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);

	putsk("Serial setup successfully");

	gdt_init();
	putsk("New GDT installed successfully");

	idt_init();
	putsk("IDT setup successfully");

	term_init();

	printk("Multiboot Info Addr: %x\n", mbd);

	unsigned long vstart = (unsigned long) &_kernel_start;
	unsigned long vend = (unsigned long) &_kernel_end;


	printk("Mem Upper: %x\n", mbd->mem_upper);
	printk("Mem Lower: %x\n\n", mbd->mem_lower);

	int i = 0;
	mmap_entry_t* entry = mbd->mmap_addr;
	while (entry < mbd->mmap_addr + mbd->mmap_length) {
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
		printk("Segment %d:\nSize: %d\nAddr: %x\nLen: %x\nType: %d\n\n", i++, entry->size,
			   (entry->base_addr_high << 16) | entry->base_addr_low, (entry->length_high << 16) | entry->length_low,
			   entry->type);
	}

	printk("Kernel Size: %dK\n", (vend - vstart) / 1024);

	printf("Welcome to the terminal!\n");

	for (;;) {
		__asm__("hlt");
	}
}
