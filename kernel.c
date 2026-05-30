#include "init_idt.h"
#include "irq.h"
#include "keyboard.h"

void kernel_main(void)
{
    init_idt();                          /* 初始化 IDT 和 PIC */

    irq_install(IRQ_KEYBOARD, keyboard_handler);  /* 注册键盘中断 */
    irq_enable(IRQ_KEYBOARD);                    /* 启用键盘中断 */

    __asm__ volatile("sti");             /* 开中断 */

    while (1) {
        __asm__ volatile("hlt");
    }
}
