#include <stdint.h>

#ifndef PORTIO_H
#define PORTIO_H

uint8_t port_inb(uint16_t);
void port_outb(uint16_t, uint8_t);

void init_serial();
void serial_print(char *);

#endif
