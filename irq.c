#include "irq.h"
#include <stdint.h>

/* ---------- 内联 IO ---------- */
static inline uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(port));
    return r;
}

static inline void outb(uint16_t port, uint8_t v) {
    __asm__ volatile("outb %0,%1"::"a"(v),"Nd"(port));
}

/* ---------- IRQ 状态 ---------- */
static void (*irq_handlers[IRQ_COUNT])(void);

/* ---------- IRQ 控制 ---------- */
void irq_install(uint8_t irq, void (*handler)(void)) {
    if (irq < IRQ_COUNT)
        irq_handlers[irq] = handler;
}

void irq_enable(uint8_t irq) {
    if (irq < 8) {
        outb(0x21, inb(0x21) & ~(1 << irq));
    } else if (irq < IRQ_COUNT) {
        outb(0xA1, inb(0xA1) & ~(1 << (irq - 8)));
        outb(0x21, inb(0x21) & ~0x04);
    }
}

/* ---------- IRQ 分发 ---------- */
void irq_dispatch(uint8_t irq) {
    if (irq < IRQ_COUNT && irq_handlers[irq])
        irq_handlers[irq]();
}