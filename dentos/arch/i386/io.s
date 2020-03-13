.intel_syntax noprefix

.global outb
outb:
    mov     al, [esp + 0x8]
    mov     dx, [esp + 0x4]
    out     dx, al
    ret

.global inb
inb:
    mov     dx, [esp + 0x4]
    in      al, dx
    ret
