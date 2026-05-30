[bits 32]

extern irq_dispatch

; IRQ 宏：只压入 IRQ 编号 0~15
%macro IRQ_STUB 1
global irq_stub_%1
irq_stub_%1:
    push %1
    jmp irq_common
%endmacro

IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15

irq_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, [esp + 32]   ; IRQ 号
    push eax
    call irq_dispatch
    add esp, 4

    ; EOI
    mov al, 0x20
    out 0x20, al
    cmp byte [esp + 32], 8
    jbe .pic1
    out 0xA0, al

.pic1:
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 4
    iret