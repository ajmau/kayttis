#ifndef TERMINAL_H
#define TERMINAL_H
#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY 0xB0000
#define VGA_WIDTH = 80
#define VGA_HEIGHT = 25

typedef struct terminal {
	size_t row;
	size_t column;
	uint8_t color;
	uint16_t* buffer;
} terminal_t;

void putc(char c);
void puts(char *string);
void init_terminal();

#endif
