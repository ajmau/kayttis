#ifndef IDT_H
#define IDT_H

#include <stdint.h>

extern void idt_load();

void install_isr();
void idt_register_gate(uint16_t n, uint64_t handler);
void idt_init();

#endif

