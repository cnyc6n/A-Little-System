#include "init_idt.h"
#include "irq.h"
#include "driver/keyboard.h"

void kernel_main(void)
{
    /* 1️⃣ 先关中断（防御性） */
    __asm__ volatile("cli");

    /* 2️⃣ 初始化 IDT（必须最先） */
    init_idt();

    /* 3️⃣ 注册 IRQ 回调 */
    irq_install(IRQ_KEYBOARD, keyboard_irq);

    /* 4️⃣ 初始化 PIC（不急着开） */
    init_pic();

    /* 5️⃣ 开 IRQ（PIC 层面） */
    irq_enable(IRQ_KEYBOARD);

    /* 6️⃣ 最后才开中断 */
    __asm__ volatile("sti");

    while (1)
        __asm__ volatile("hlt");
}