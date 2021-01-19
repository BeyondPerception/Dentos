.intel_syntax noprefix

// Invalidate a single page in the TLB
.global invlpg
invlpg:
    push    ebp
    mov     ebp, esp

    mov     eax, dword ptr[ebp+8]
    invlpg  [eax]

    leave
    ret

// Do a complete reload of the TLB
.global flushtlb
flushtlb:
    push    ebp
    mov     ebp, esp

    mov     eax, cr3
    mov     cr3, eax

    leave
    ret