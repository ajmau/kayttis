#include "portio.h"
#include "memory/pmm.h"
#include "interrupts/idt.h"
#include "interrupts/isr.h"
#include "stivale.h"

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}

void kmain(stivale_info_t *info)
{
	init_serial();
	init_pmm(info);
	isr_init();
	idt_init();

	int *a = (int*)pmm_alloc(0x1000);
	int *b = (int*)pmm_alloc(0x1000*10);
	int *c = (int*)pmm_alloc(0x1000);


	char number[10];
	itoa(a, number, 10);

	serial_print("a = ");
	serial_print(number);
	serial_print("\n");

	itoa(b, number, 10);

	serial_print("b = ");
	serial_print(number);
	serial_print("\n");

	itoa(c, number, 10);

	serial_print("c = ");
	serial_print(number);
	serial_print("\n");


	write_string(2, "hoi siieeee");

}
