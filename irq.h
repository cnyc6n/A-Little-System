#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

#define IRQ_TIMER    0
#define IRQ_KEYBOARD 1
#define IRQ_MOUSE   12

#define IRQ_BASE    32
#define IRQ_COUNT   16

void irq_install(uint8_t irq, void (*handler)(void));
void irq_enable(uint8_t irq);
void irq_dispatch(uint8_t irq);

#endif
