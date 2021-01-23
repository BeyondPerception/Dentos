#include <stdio.h>
#include <stddef.h>

#include <kernel/multiboot.h>
#include <kernel/ptables.h>
#include <kernel/kernel.h>
#include <kernel/serial.h>

unsigned int* stack_top = (unsigned int*) 0xFF800000;
unsigned int* const stack_bottom = (unsigned int*) 0xFF800000;

void* next_page() {
	if (stack_top < stack_bottom) {
		// OUT OF MEMORY!!!
		return NULL;
	}
	return (void*) *stack_top--;
}

void free_page(void* addr) {
	*(++stack_top) = (unsigned int) addr;
}

static void map_stack(unsigned int phys_addr) {
	// Reserve the 1022nd page table for the page stack. Stick it in physical memory right after the page tables.
	unsigned int virt_addr = (unsigned int) stack_top;
	for (int i = 0; i < 1024; i++) {
		map_page((void*) phys_addr, (void*) virt_addr, PTABLES_WRITABLE);
		phys_addr += 4096;
		virt_addr += 4096;
	}
}

multiboot_info_t* mb_info;
unsigned int mb_start;
unsigned int mb_end;

static void setup_stack(unsigned int map_addr) {
	map_stack(map_addr);

	// Add all the usable addresses given to us by GRUB to the stack.
	multiboot_memory_map_t* entry = (multiboot_memory_map_t*) mb_info->mmap_addr;
	while ((unsigned int) entry < (mb_info->mmap_addr + mb_info->mmap_length)) {
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
		if (entry->type != MULTIBOOT_MEMORY_AVAILABLE) {
			continue;
		}

		unsigned int start_addr = entry->addr_high << 16 | entry->addr_low;
		unsigned int end_addr = start_addr + (entry->len_high << 16 | entry->len_low);
		if ((start_addr & 0xFFFFF000) != start_addr) {
			// If the addresses isn't page aligned, jump to the next page aligned address.
			start_addr = (start_addr + 0x400) & 0xFFFFF000;
			if (start_addr >= end_addr) {
				continue;
			}
		}
		for (unsigned i = start_addr; i < end_addr; i += 4096) {
			*stack_top = i;
			stack_top++;
		}
	}
	printf("Top of stack: %p\n", stack_top);
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