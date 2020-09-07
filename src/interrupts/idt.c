#include <stdint.h>
#include "idt.h"
#include "../portio.h"
typedef struct {
	uint16_t size;
	uint64_t offset; } __attribute__ ((packed)) idtr_t;

typedef struct idt_gate_options
{
  uint8_t index : 3;
  uint8_t reserved : 5;
  uint8_t type : 1;
  uint8_t ones : 3;
  uint8_t zero : 1;
  uint8_t DPL : 2;
  uint8_t present : 1;
} __attribute__((packed)) idt_entry_options_t;

typedef struct {
	uint16_t offset_low16;
	uint16_t cs;
	uint8_t ist;
//	idt_entry_options_t attributes;
	uint8_t type_attr;
	uint16_t offset_mid16;
	uint32_t offset_high32;
	uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_register {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idt_register_t;

idt_entry_t IDT[256];
idt_register_t idt_reg;

void idt_register_gate(uint16_t n, uint64_t handler) {
	IDT[n].offset_low16 = (uint16_t)handler;
	IDT[n].offset_mid16 = (uint16_t)(handler >> 16);
	IDT[n].offset_high32 = (uint32_t)(handler >> 32);

	IDT[n].zero = 0;
	IDT[n].cs = 0x08;

/*	IDT[n].attributes.index = 0;
	IDT[n].attributes.type = 1;
	IDT[n].attributes.ones = 0x07;
	IDT[n].attributes.zero = 0;
	IDT[n].attributes.DPL = 0;
	IDT[n].attributes.present = 1;*/
	IDT[n].type_attr = 0x8e;
}

extern void idt_load();

void idt_init()
{
	serial_print("idt_init()\n");
	idt_reg.base = (uint64_t)&IDT;
	idt_reg.limit = 256 * sizeof(idt_entry_t) - 1;
	idt_load();
	serial_print("idt loaded\n");

}
