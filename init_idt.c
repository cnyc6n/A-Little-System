#include "irq.h"

extern void irq_stub_0(void);
extern void irq_stub_1(void);
extern void irq_stub_12(void);

static inline void outb(uint16_t p, uint8_t v) {
    __asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p));
}

struct IDTEntry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct IDTEntry idt[256];
static struct IDTPtr idtp;

static void idt_set_gate(uint8_t n, uint32_t handler) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].selector  = 0x08;
    idt[n].flags     = 0x8E;
    idt[n].reserved  = 0;
}

void init_pic(void) {
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA0, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void init_idt(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)idt;

    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0);

    idt_set_gate(IRQ_BASE + IRQ_TIMER,    (uint32_t)irq_stub_0);
    idt_set_gate(IRQ_BASE + IRQ_KEYBOARD, (uint32_t)irq_stub_1);
    idt_set_gate(IRQ_BASE + IRQ_MOUSE,    (uint32_t)irq_stub_12);

    init_pic();
    __asm__ volatile("lidt %0" :: "m"(idtp));
}