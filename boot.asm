[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

start:
    ; ===== 段寄存器 & 栈 =====
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7BFE

    ; ===== 清屏 =====
    mov ah, 0x06
    mov al, 0
    mov ch, 0
    mov cl, 0
    mov dh, 24
    mov dl, 79
    mov bh, 0x07
    int 0x10

    ; ===== 光标复位 =====
    mov ah, 0x02
    mov bh, 0
    mov dx, 0
    int 0x10

    ; ===== 提示按任意键 =====
    mov si, press_key_msg
    call print16

    ; ===== 等待任意键 =====
    xor ah, ah
    int 0x16

    ; 显示按下的键
    mov ah, 0x0E
    int 0x10

    mov si, newline
    call print16

    ; ===== 加载内核提示 =====
    mov si, loading_msg
    call print16

    ; ===== 保存驱动器号 =====
    mov [boot_drive], dl

    ; ===== 重置磁盘 =====
    xor ah, ah
    mov dl, [boot_drive]
    int 0x13

    ; ===== 读取内核 =====
    mov bx, KERNEL_OFFSET
    mov ah, 0x02
    mov al, 16
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; ================================
    ; 进入保护模式（最终安全版）
    ; ================================

    cli                     ; 关中断
    call enable_a20        ; ✅ 开启 A20

    ; ✅ 16 位安全 LGDT（NASM 专用）
    db 0x66
    lgdt [gdt_desc]

    ; ✅ 设置 PE 位
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; ✅ 远跳转，加载 CS
    jmp 0x08:protected_mode

[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; ✅ 跳转到 kernel_main
    jmp KERNEL_OFFSET

.halt:
    hlt
    jmp .halt

; ========================
; A20 开启
; ========================
enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

; ========================
; 实模式打印
; ========================
print16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print16
.done:
    ret

disk_error:
    mov si, disk_error_msg
    call print16
    cli
    hlt

; ========================
; 数据
; ========================
press_key_msg   db "Press any key to start...", 0
loading_msg     db "Loading kernel...", 0x0D,0x0A,0
newline         db 0x0D,0x0A,0
disk_error_msg  db "Disk read error!",0
boot_drive      db 0

; ========================
; GDT
; ========================
gdt_start:
    dq 0x0000000000000000      ; null
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
gdt_end:

; ✅ NASM + ORG 0x7C00 专用 GDT 描述符
gdt_desc:
    dw gdt_end - gdt_start - 1
    dw 0x7C00 + gdt_start
    dw 0x0000

times 510 - ($-$$) db 0
dw 0xAA55