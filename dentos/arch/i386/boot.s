.intel_syntax noprefix

// Some magic numbers that need to be included
// for grub to recognize the multiboot
.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set MAGIC, 0x1BADB002
.set FLAGS, ALIGN | MEMINFO
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot.data, "a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// Setup the initial stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
	.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
.align 4096
boot_page_directory:
    .skip 4096
boot_page_table1:
    .skip 4096
kernel_page_table:
    .skip 4096

.extern	kernel_main

.section .multiboot.text, "ax"
.global _start
_start:
    // Identity map the 1st MB and then to the end of the kernel.
    mov     edi, OFFSET kernel_page_table - 0xC0000000 // Physical address of the page table that will map the kernel (start of table)
    mov     esi, 0 // Location of current page

    // Map 1023 pages (or until the end of the kernel), the 1024th will be the VGA buffer
    mov     ecx, 1023
1:
	// Only map the kernel
    cmp     esi, OFFSET _kernel_end - 0xC0000000
    jge     3f

    // Or-ing with 3 marks the entry as present and read/writable
    mov     edx, esi
    or      edx, 0x3
    mov     DWORD PTR[edi], edx
2:
    // Page size is 4096 bytes
    add     esi, 4096
    // kernel_page_table entry size is 4 bytes
    add     edi, 4
    loop    1b
3:
    // Map VGA video memory to 0xC03FF000 as "present, writable".
    mov     DWORD PTR[kernel_page_table - 0xC0000000 + 1023 * 4], 0x000B8000 | 0x3

	// The page table is used at both page directory entry 0 (virtually from 0x0
	// to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	// 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	// higher half). The kernel is identity mapped because enabling paging does
	// not change the next instruction, which continues to be physical. The CPU
	// would instead page fault if there was no identity mapping.

    // Map the page table to virtual addresses 0x00000000 and 0xC0000000
    // Load the page tables into the page directory
    mov     DWORD PTR[boot_page_directory - 0xC0000000 + 0], OFFSET kernel_page_table - 0xC0000000 + 0x3
	mov		DWORD PTR[boot_page_directory - 0xC0000000 + 768 * 4], OFFSET kernel_page_table - 0xC0000000 + 0x3

	mov		ecx, OFFSET boot_page_directory - 0xC0000000
	mov		cr3, ecx

	// Enable paging
	mov		ecx, cr0
	or		ecx, 0x80010000
	mov		cr0, ecx

	// Jump to higher half
	lea		ecx, 4f
	jmp		ecx

.section .text
4:
	// Paging is now fully setup and enabled

	// Reload cr3 to force a TLB flush so the changes to take effect.
	mov		ecx, cr3
	mov		cr3, ecx

    // Start the stack pointer at the top of the stack to meet C specs
	mov		esp, OFFSET stack_top

	call    pd_init

    // Push memory map information from GRUB onto stack for kernel
	push    eax
	push    ebx

    // Call global constructors
	call	_init

	// Pass control to the kernel
	call	kernel_main

	// If kernel_main unexpectedly returns
1:
	cli // Disable CPU interrupts
	hlt	// Halt the CPU
	jmp     1b // Loop if the last two insturctions failed
//.size _start, . - _start

// secondary page directory initalization
.global pd_init
pd_init:
    // Clear the kernel mapping at the begining of memory
    mov     DWORD PTR[boot_page_directory], 0

    // Put the address of the page directory itself into the last page directory entry for easy access in C code.
    mov     DWORD PTR[boot_page_directory + 1023 * 4], OFFSET boot_page_directory + 0x3

    // Identity map the first 256 pages (1MB)
    mov     ecx, 256
    mov     edi, OFFSET boot_page_table1
    mov     esi, 0
1:
    mov     edx, esi
    or      edx, 0x3
    mov     DWORD PTR[edi], edx

    add     esi, 4096
    add     edi, 4

    loop    1b

    mov     DWORD PTR[boot_page_directory], OFFSET boot_page_table1 - 0xC0000000 + 0x3
    ret

gdtr:
    .short  0 // size of gdt
    .int    0 // location of gdt

.global gdt_set
gdt_set:
    mov     eax, [esp + 4]
    mov     [gdtr + 2], eax
    mov     ax, [esp + 8]
    mov     [gdtr], ax
    lgdt    [gdtr]
    ret

// Setup new segment registers.
.global gdt_flush
gdt_flush:
    jmp     0x08:.flushr
.flushr:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    ret

idtr:
    .short  0 // size of idt
    .int    0 // location of idt

.global idt_set
idt_set:
    mov     eax, [esp + 4]
    mov     [idtr + 2], eax
    mov     ax, [esp + 8]
    mov     [idtr], ax
    lidt    [idtr]
    sti
    ret
