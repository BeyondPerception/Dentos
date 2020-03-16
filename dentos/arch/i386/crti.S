.intel_syntax noprefix

.section .init
.global _init
.type _init, @function
_init:
	/* Make sure that functions have a preamble*/
	push    ebp
	mov     ebp, esp

.section .fini
.global _fini
.type _fini, @function
_fini:
	push    ebp
	mov     ebp, esp
