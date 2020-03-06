/* This file gets linked at the end of all executables to make sure they actually return */
.intel_syntax noprefix

.section .init
	pop		ebp
	ret

.section .fini
	pop		ebp
	ret
