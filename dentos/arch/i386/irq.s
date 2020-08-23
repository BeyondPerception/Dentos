.intel_syntax noprefix

.extern irq0_handler
.global irq0
irq0:
    pushad
    call    irq0_handler
    popad
    iret
