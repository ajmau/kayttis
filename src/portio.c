#include <stdint.h>

#define COM1 0x3f8

uint8_t port_inb(uint16_t port)
{
	uint8_t ret;
	asm("in %%dx, %%al" : "=a"(ret) : "d"(port));
	return ret;
}

void port_outb(uint16_t port, uint8_t data)
{
	asm("out %%al, %%dx" : : "a"(data), "d"(port));
}

void serial_wait_for_transmit()
{
	while (!(port_inb(COM1 + 5) & 0x20)) { asm("pause"); }
}

void init_serial()
{
	port_outb(COM1 + 1, 0); // disable interrupts
	port_outb(COM1 + 3, 1<<7);

	port_outb(COM1 + 0, 3);
	port_outb(COM1 + 1, 0);

	port_outb(COM1 + 3, 3);
	port_outb(COM1 + 2, 0xC7);
	port_outb(COM1 + 4, 0x0B);

	serial_wait_for_transmit();
}

void serial_print(char *string)
{
	while (*string != '\0') {
		serial_wait_for_transmit();
		port_outb(COM1, *string);
		string++;
	}
}
