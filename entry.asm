; entry.asm
[bits 32]
[global _start]

section .text
; 确保 _start 是第一个字节
_start:
    ; 设置栈指针
    mov esp, 0x90000
    mov ebp, esp
    
    ; 调用 C 主函数
    call kernel_main
    
    ; 不应该返回
    cli
    hlt

; 声明外部符号
[extern kernel_main]