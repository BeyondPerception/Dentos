.section .init
.global _init
.type _init, @function
_init:
	/* Make sure that functions have a preamble*/
	push %ebp
	movl %esp, %ebp

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
