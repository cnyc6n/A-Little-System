[bits 32]
[global load_idt]

load_idt:
    cli
    mov eax, [esp+4]
    lidt [eax]
    ret