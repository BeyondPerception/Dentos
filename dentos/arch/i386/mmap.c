#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/ptables.h>
#include <kernel/kernel.h>
#include <kernel/serial.h>

void* next_page() {

}

void* fetch_pages(unsigned int n) {

}

void free_page(void* addr) {

}

void free_pages(void* addr) {

}

multiboot_info_t* mb_info;
unsigned int mb_start;
unsigned int mb_end;

static void map_stack(unsigned int phys_addr) {
	// Reserve the 1022nd page table for the page stack. Stick it in physical memory right after the page tables.
	unsigned int virt_addr = 0xFF800000;
	for (int i = 0; i < 1024; i++) {
		printk("Phys: %p, Virt: %p\n", phys_addr, virt_addr);
		map_page((void*) phys_addr, (void*) virt_addr, 0b10);
		phys_addr += 4096;
		virt_addr += 4096;
	}
}

static void setup_stack(unsigned int start_addr) {
	map_stack(start_addr);
	print_page_dir(1022);
}

void palloc_init(multiboot_info_t* multiboot_info) {
	if ((multiboot_info->flags & 1 << 6) == 0) {
		puts("Error: no multiboot memory map provided!");
		panic();
	}

	mb_info = multiboot_info;
	mb_start = mb_info;
	mb_end = mb_info + sizeof(multiboot_info_t);

	unsigned int ptables_end = ptables_init();
	printf("%x\n", ptables_end);

	setup_stack(ptables_end);
}