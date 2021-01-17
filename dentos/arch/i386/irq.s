.intel_syntax noprefix

.extern divbyzero_handler
.global irq0
irq0:
    pushad
    call    divbyzero_handler
    popad
    iret

.extern debug_handler
.global irq1
irq1:
    pushad
    call    debug_handler
    popad
    iret

.extern nonmaskable_handler
.global irq2
irq2:
    pushad
    call    nonmaskable_handler
    popad
    iret

.extern breakpoint_handler
.global irq3
irq3:
    pushad
    call    breakpoint_handler
    popad
    iret

.extern overflow_handler
.global irq4
irq4:
    pushad
    call    overflow_handler
    popad
    iret

.extern boundexceded_handler
.global irq5
irq5:
    pushad
    call    boundexceded_handler
    popad
    iret

.extern invalopcode_handler
.global irq6
irq6:
    pushad
    call    invalopcode_handler
    popad
    iret

.extern devnotavail_handler
.global irq7
irq7:
    pushad
    call    devnotavail_handler
    popad
    iret

.extern doublefault_handler
.global irq8
irq8:
    pushad
    call    doublefault_handler
    pop     eax
    popad
    iret

.extern invalidtss_handler
.global irq10
irq10:
    pushad
    call    invalidtss_handler
    pop     eax
    popad
    iret

.extern seg_notprsnt_handler
.global irq11
irq11:
    pushad
    call    seg_notprsnt_handler
    pop     eax
    popad
    iret

.extern stack_segfault_handler
.global irq12
irq12:
    pushad
    call    stack_segfault_handler
    pop     eax
    popad
    iret

.extern gpf_handler
.global irq13
irq13:
    pushad
    call    gpf_handler
    pop     eax
    popad
    iret

.extern pagefault_handler
.global irq14
irq14:
    push    ebp
    mov     ebp, esp
    pushad

    // +4 instead of +8 becuase the error code is pushed after the return addr b/c this is an interrupt handler
    mov     esi, dword ptr[ebp+4]
    push    esi # push error code
    mov     edi, cr2
    push    edi # push memory address
    call    pagefault_handler
    add     esp, 8 # clean up params

    popad
    leave
    add     esp, 4 # clean up error code
    iret

.extern x87float_exc_handler
.global irq16
irq16:
    pushad
    call    x87float_exc_handler
    popad
    iret

.extern align_chk_handler
.global irq17
irq17:
    pushad
    call    align_chk_handler
    pop     eax
    popad
    iret

.extern machine_chk_handler
.global irq18
irq18:
    pushad
    call    machine_chk_handler
    popad
    iret

.extern SIMDfloat_exc_handler
.global irq19
irq19:
    pushad
    call    SIMDfloat_exc_handler
    popad
    iret

.extern virtualization_exc_handler
.global irq20
irq20:
    pushad
    call    virtualization_exc_handler
    popad
    iret

.extern security_exc_handler
.global irq30
irq30:
    pushad
    call    security_exc_handler
    pop     eax
    popad
    iret

.extern timer_handler
.global irq32
irq32:
    pushad
    call    timer_handler
    popad
    iret

.extern keyboard_handler
.global irq33
irq33:
    pushad
    call    keyboard_handler
    popad
    iret
