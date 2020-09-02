/* The bootloader will start execution at the symbol designated as the entry point. In this case, that's '_start' (defined in boot.s) */
ENTRY(_start)

KERNEL_VIRTUAL_BASE = 0xC0000000;

SECTIONS
{
    /* The kernel will live at 3GB + 1MB in the virtual address space, */
	/* which will be mapped to 1MB(below 1MB is GRUB and BIOS code) in the physical address space. */
	/* Note that we page-align the sections. */
    . = 0x00100000;

    .multiboot.data : {
        *(.multiboot.data)
    }

    .multiboot.text ALIGN (4k) : {
        *(.multiboot.text)
    }

    . += KERNEL_VIRTUAL_BASE;

    /* Define symbol to be used in the assembly */
    _kernel_start = .;

	/* Mutliboot header goes first so the bootloader recognizes the file propely */
	.text ALIGN (4K) : AT (ADDR (.text) - KERNEL_VIRTUAL_BASE) {
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - KERNEL_VIRTUAL_BASE) {
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN (4K) : AT (ADDR (.data) - KERNEL_VIRTUAL_BASE) {
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN (4K) : AT (ADDR (.bss) - KERNEL_VIRTUAL_BASE) {
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	/* End address symbol used in the assembly */
	_kernel_end = .;
}
