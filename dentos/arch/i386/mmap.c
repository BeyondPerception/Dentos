#include <kernel/multiboot.h>
#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/serial.h>

// Constant address because we put the address of the page directory into the last entry.
// This maps all of the page table entries and the page directory. We can then access the page directory by calculating its address.
// It is the same formula we will use to translate physical addresses into virtual addresses: pd_index * (4096 * 1024) + pte_index * 4096
// So, 1023 * (4096*1024) + 1023 * 4096 = 0xFFFFF000. (4096*1024 is equal to 4MB, the granularity of a page directory entry. 4096 is 1KB, the granularity of a page table entry).
const unsigned int PD_LOCATION = 0xFFFFF000;
unsigned int* pd = (unsigned int*) PD_LOCATION;

void map_page(void* phys_addr, void* virt_addr, int flags) {
	int pd_index = ((unsigned int)virt_addr >> 22);
	int pt_index = ((unsigned int)virt_addr & 0xFF000) / 0x1000;

	printk("PD_Index: %d\n", pd_index);
	printk("PT_Index: %d\n", pt_index);
}

multiboot_info_t* mb_info;
unsigned int mb_start;
unsigned int mb_end;

void mmap_init(multiboot_info_t* multiboot_info) {
	if ((multiboot_info->flags & 1 << 6) == 0) {
		puts("Error: no multiboot memory map provided!");
		panic();
	}
	mb_info = multiboot_info;
	mb_start = mb_info;
	mb_end = mb_info + sizeof(multiboot_memory_map_t);

	map_page((void*) 0x1000, (void*) 0xF012312, 0);

	multiboot_memory_map_t* mmap_entry = mb_info->mmap_addr;
//	while (mmap_entry < mb_info->mmap_addr + mb_info->mmap_length) {
//	}
}