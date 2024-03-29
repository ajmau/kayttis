#include "../portio.h"
#include "idt.h"
#include "isr.h"

const char* exception_messages[] = { "Division By Zero",
                                     "Debug",
                                     "Non Maskable Interrupt",
                                     "Breakpoint",
                                     "Into Detected Overflow",
                                     "Out of Bounds",
                                     "Invalid Opcode",
                                     "No Coprocessor",

                                     "Double Fault",
                                     "Coprocessor Segment Overrun",
                                     "Bad TSS",
                                     "Segment Not Present",
                                     "Stack Fault",
                                     "General Protection Fault",
                                     "Page Fault",
                                     "Unknown Interrupt",

                                     "Coprocessor Fault",
                                     "Alignment Check",
                                     "Machine Check",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",

                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved",
                                     "Reserved" };

void isr_handler(uint64_t id) 
{
	serial_print("isr_handler()\n");
	serial_print(exception_messages[id]);
	serial_print("\n");
}

void isr_init() 
{
  idt_register_gate(0, (uint64_t)isr0);
  idt_register_gate(1, (uint64_t)isr1);
  idt_register_gate(2, (uint64_t)isr2);
  idt_register_gate(3, (uint64_t)isr3);
  idt_register_gate(4, (uint64_t)isr4);
  idt_register_gate(5, (uint64_t)isr5);
  idt_register_gate(6, (uint64_t)isr6);
  idt_register_gate(7, (uint64_t)isr7);
  idt_register_gate(8, (uint64_t)isr8);
  idt_register_gate(9, (uint64_t)isr9);
  idt_register_gate(10, (uint64_t)isr10);
  idt_register_gate(11, (uint64_t)isr11);
  idt_register_gate(12, (uint64_t)isr12);
  idt_register_gate(13, (uint64_t)isr13);
  idt_register_gate(14, (uint64_t)isr14);
  idt_register_gate(15, (uint64_t)isr15);
  idt_register_gate(16, (uint64_t)isr16);
  idt_register_gate(17, (uint64_t)isr17);
  idt_register_gate(18, (uint64_t)isr18);
  idt_register_gate(19, (uint64_t)isr19);
  idt_register_gate(20, (uint64_t)isr20);
  idt_register_gate(21, (uint64_t)isr21);
  idt_register_gate(22, (uint64_t)isr22);
  idt_register_gate(23, (uint64_t)isr23);
  idt_register_gate(24, (uint64_t)isr24);
  idt_register_gate(25, (uint64_t)isr25);
  idt_register_gate(26, (uint64_t)isr26);
  idt_register_gate(27, (uint64_t)isr27);
  idt_register_gate(28, (uint64_t)isr28);
  idt_register_gate(29, (uint64_t)isr29);
  idt_register_gate(30, (uint64_t)isr30);
  idt_register_gate(31, (uint64_t)isr31);
}

