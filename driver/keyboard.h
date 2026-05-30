#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_irq(void);
void keyboard_process_scancode(uint8_t sc);

int  keyboard_has_data(void);
uint8_t keyboard_read_scancode(void);
char scancode_to_ascii(uint8_t sc);

#endif
