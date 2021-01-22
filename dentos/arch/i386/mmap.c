#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/ptables.h>
#include <kernel/kernel.h>

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

static void setup_stack(unsigned int start_addr) {
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

	setup_stack(ptables_end);
}