#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

#define IRQ_TIMER    0
#define IRQ_KEYBOARD 1
#define IRQ_MOUSE   12

#define IRQ_BASE    32
#define IRQ_COUNT   16

/* ---------- IRQ stub 函数声明（来自 stub.asm） ---------- */
extern void irq_stub_0(void);
extern void irq_stub_1(void);
extern void irq_stub_2(void);
extern void irq_stub_3(void);
extern void irq_stub_4(void);
extern void irq_stub_5(void);
extern void irq_stub_6(void);
extern void irq_stub_7(void);
extern void irq_stub_8(void);
extern void irq_stub_9(void);
extern void irq_stub_10(void);
extern void irq_stub_11(void);
extern void irq_stub_12(void);
extern void irq_stub_13(void);
extern void irq_stub_14(void);
extern void irq_stub_15(void);

/* ---------- IRQ 控制函数 ---------- */
void irq_install(uint8_t irq, void (*handler)(void));
void irq_enable(uint8_t irq);
void irq_dispatch(uint8_t irq);

#endif
