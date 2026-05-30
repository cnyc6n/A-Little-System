#include "keyboard.h"
#include <stdint.h>

#define KB_SIZE 16
static uint8_t kb_buf[KB_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

static inline uint8_t inb(uint16_t p) {
    uint8_t r;
    __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(p));
    return r;
}

void keyboard_irq(void) {
    uint8_t sc = inb(0x60);
    keyboard_process_scancode(sc);
}

void keyboard_process_scancode(uint8_t sc) {
    int next = (head + 1) % KB_SIZE;
    if (next != tail) {
        kb_buf[head] = sc;
        head = next;
    }
}

/* ===== 公共接口 ===== */
int keyboard_has_data(void) {
    return head != tail;
}

uint8_t keyboard_read_scancode(void) {
    if (head == tail) return 0;
    uint8_t sc = kb_buf[tail];
    tail = (tail + 1) % KB_SIZE;
    return sc;
}

static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

char scancode_to_ascii(uint8_t sc) {
    if (sc >= 128) return 0;
    return keymap[sc];
}
