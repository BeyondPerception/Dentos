.intel_syntax noprefix

.extern	kernel_main

.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

.section .multiboot
.align 4
.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM

.section .bss
.align 16
stack_bottom:
	.skip 4096
stack_top:

.section .text
.global start

start:
	// Start the stack pointer at the top of the stack to meet C reqs
	mov		stack_top, esp

	call	kernel_main

	hang:
		cli // Disable CPU interrupts
		hlt	// Halt the CPU
		jmp hang // Loop if the last two insturctions failed