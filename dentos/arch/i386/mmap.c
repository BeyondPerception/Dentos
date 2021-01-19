#include <kernel/multiboot.h>
#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/serial.h>

// Constant address because we put the address of the page directory into the last entry.
// This maps all of the page table entries and the page directory as the last entry of the page directory. We can then access the page directory by calculating its address.
// It is the same formula we will use to translate physical addresses into virtual addresses: pd_index * (4096 * 1024) + pt_index * 4096
// So, 1023 * (4096*1024) + 1023 * 4096 = 0xFFFFF000. (4096*1024 is equal to 4MB, the granularity of a page directory entry. 4096 is 4KB, the granularity of a page table entry).
const unsigned int PD_LOCATION = 0xFFFFF000;
unsigned int* pd = (unsigned int*) PD_LOCATION;

// Now this address is 4MB from the end of the address space (0x100000000 - 0x400000) because we mapped all of the page tables into the last page directory entry.
// An alternative way to calculate this is using the same formula as above. (pd_index * (4096 * 1024) + pt_index * 4096)
// Plugging in numbers: 1023 * (4096 * 1024) + 0 * 4096 = 0xFFC00000. This is why, although slightly counter-intuitive, the pt is before the pd in memory. Because the pd is acting like a pt, just the very last one.
const unsigned int PT_LOCATION = 0xFFC00000;
unsigned int* pt_base = (unsigned int*) PT_LOCATION;

static void* virt2phys(void* virt_addr) {
	int pd_index = ((unsigned int) virt_addr >> 22);
	int pt_index = ((unsigned int) virt_addr & 0xFF000) / 0x1000;
	printf("Calc PD Index: %d\nCalc PT Index: %d\n", pd_index, pt_index);
	unsigned int* pt = pt_base + pd_index * 1024;

	return (void*) ((pt[pt_index] & 0xFFFFF000) | ((unsigned int) virt_addr & 0xFFF));
}

__attribute__((unused))
static void print_page_dir(int pd_index, int pt_index) {
	unsigned int pde = pd[pd_index];
	unsigned int* pt = pt_base + pd_index * 1024;

	putsk("Page Directory:");
	for (int i = 0; i < 1024; i++) {
		char str[34];
		str[33] = '\0';

		pde = pd[i];

		for (int j = 32; j >= 0; j--) {
			if (j == 20) {
				str[j] = '-';
				continue;
			}
			str[j] = (pde & 1) + '0';
			pde >>= 1;
		}

		printk("%04d-%p: %s\n", i, &pd[i], str);
	}
	if (pt_index == -1) {
		return;
	}
	putsk("---------------------------------------");
	printk("Page Table #%d:\n", pd_index);
	for (int i = 0; i < 1024; i++) {
		char str[34];
		str[33] = '\0';

		int pte = pt[i];

		for (int j = 32; j >= 0; j--) {
			if (j == 20) {
				str[j] = '-';
				continue;
			}
			str[j] = (pte & 1) + '0';
			pte >>= 1;
		}

		printk("%04d-%p: %s\n", i, &pt[i], str);
	}
}

int map_page(void* phys_addr, void* virt_addr, int flags) {
	if (((unsigned int) phys_addr & 0xFFFFF000) != (unsigned int) phys_addr ||
		((unsigned int) virt_addr & 0xFFFFF000) != (unsigned int) virt_addr) {
		// Addresses are not page aligned
		return -1;
	}

	int pd_index = ((unsigned int) virt_addr >> 22);
	int pt_index = ((unsigned int) virt_addr & 0xFF000) / 0x1000;

	unsigned int pde = pd[pd_index];
	unsigned int* pt = pt_base + pd_index * 1024;

	if (!(pde & 1)) {
		pd[pd_index] |= 3;
	}
	pt[pt_index] = ((unsigned int) phys_addr) | (flags & 0xFFF) | 1;

	putsk("Page Directory:");
	for (int i = 0; i < 1024; i++) {
		printk("Phys Addr %d: %p\n", i, pd[i] & 0xFFFFF000);
	}

	return 0;
}

static unsigned int fill_pd() {
	unsigned int pd_addr;
	for (int i = 768; i < 1023; i++) {
		unsigned int* pt = pt_base + i * 1024;
		for (int j = 0; j < 1024; j++) {
			unsigned int pte = pt[j];
			if (!(pte & 1)) {
				pd_addr = (pt[j - 1] & 0xFFFFF000) + 0x1000;
				goto found;
			}
		}
	}
	found:
	for (int i = 1; i < 1023; i++) {
		if (i == 768) {
			continue;
		}
		pd[i] = pd_addr;
		pd_addr += 0x1000;
	}

	return pd_addr;
}

multiboot_info_t* mb_info;
unsigned int mb_start;
unsigned int mb_end;

unsigned int reserved_start;
unsigned int reserved_end;

void mmap_init(multiboot_info_t* multiboot_info, unsigned int kernel_start) {
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