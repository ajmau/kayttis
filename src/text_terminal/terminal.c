#include "terminal.h"
#include "../portio.h"

terminal_t terminal = {0, 0, 0, 0};

size_t row;
size_t column;
uint8_t color;
volatile char *buffer;

void putc(char c)
{
	if (c == '\n') {
		serial_print("newline\n");
		column = 0;
		row++;
		return;
	}

	uint8_t offset = row * 80 + column;
        *(buffer+offset) = c;
        *(buffer+offset+1) = 2;

	column+=2;
	if (column > 80) {
		column = 0;
		row++;
	}

}

void puts(char *string)
{
	for (size_t i = 0; *(string+i) != '\0'; i++)
		putc(string[i]);
}

void init_terminal()
{
	color = 2;
	column = 0;
	row = 5;
	buffer = (volatile char *)0xb8000;
	serial_print("init_terminal()\n");
}
