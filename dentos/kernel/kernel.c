#include <stdio.h>
#include <stdlib.h>

#include <kernel/multiboot.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/mmap.h>
#include <kernel/kernel.h>

extern void _kernel_start(void);
extern void _kernel_end(void);

// Main method called by boot
_Noreturn void kernel_main(multiboot_info_t* mb_info, unsigned int mb_magic) {
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

	puts(" _   _ _    ____             _            \n" \
                "| | | (_)  |  _ \\  ___ _ __ | |_ ___  ___ \n" \
                "| |_| | |  | | | |/ _ \\ '_ \\| __/ _ \\/ __|\n" \
                "|  _  | |  | |_| |  __/ | | | || (_) \\__ \\\n" \
                "|_| |_|_|  |____/ \\___|_| |_|\\__\\___/|___/\n" \
                "");

	if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		puts("Error: not loaded by a multiboot compliant bootloader!");
		panic();
	}

	mmap_init(mb_info);

	printk("Multiboot Info Addr: %p\n", mb_info);

	unsigned long vstart = (unsigned long) &_kernel_start;
	unsigned long vend = (unsigned long) &_kernel_end;

	int i = 0;
	multiboot_memory_map_t* entry = mb_info->mmap_addr;
	while (entry < mb_info->mmap_addr + mb_info->mmap_length) {
		//printf("Struct Addr: %p\n", entry);
		entry = (multiboot_memory_map_t*) ((unsigned long) entry + entry->size + sizeof(entry->size));
		printk("Segment %d:, Size: %d, Addr: %p, Len: %x, Type: %d\n", i++, entry->size,
			   (entry->addr_high << 16) | entry->addr_low, (entry->len_high << 16) | entry->len_low,
			   entry->type);
	}

	printk("Kernel Start: %p\n", vstart);
	printk("Kernel End: %p\n", vend);
	printk("Kernel Size: %dK\n", (vend - vstart) / 1024);

	puts("Welcome to the Dentos Console!\n");

	for (;;) {
		__asm__("hlt");
	}
}
