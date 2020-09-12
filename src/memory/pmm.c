#include <stdint.h>
#include "../stivale.h"
#include "../portio.h"
#include "mem.h"

uint64_t memory_pages;
uint64_t *bitmap;

char * itoa3( int value, char * str, int base )
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


void pmm_set_page_used(uint64_t page)
{
	uint64_t byte = page / 8;
	uint64_t bit = page % 8;
	bitmap[byte] |= (1<<bit);
}

void pmm_set_page_free(uint64_t page)
{
	uint64_t byte = page / 8;
	uint64_t bit = page % 8;
	bitmap[byte] &= ~(1<<bit);
}

void init_pmm(stivale_info_t *info)
{
	serial_print("init_pmm()\n");
	mmap_entry_t *mmap = (void *) info->memory_map_addr;
	uint64_t memory_bytes = (mmap[info->memory_map_entries -1].addr + mmap[info->memory_map_entries -1].len);
	memory_pages = (memory_bytes + 0x1000 -1) / 0x1000;
	uint64_t bitmap_bytes = (memory_pages + 8 - 1) / 8;

	bitmap = (void *) 0;

	for (uint64_t i = 1; i < info->memory_map_entries; i++) {
		if (mmap[i].type == STIVALE_MEMORY_AVAILABLE) {
			if (mmap[i].len >= bitmap_bytes) {
				bitmap = (void *) (mmap[i].addr + 0xFFFF800000000000);
				break;
			}
		}
	}

	if (!bitmap) {
		serial_print("ei löytyny osoitetta bitmapille :(\n");
		while (1) {
			asm volatile("hlt");
		}
	}

	memset(bitmap, 0xff, bitmap_bytes);
	
	for (uint64_t i = 1; i < info->memory_map_entries; i++) {
		if (mmap[i].type == STIVALE_MEMORY_AVAILABLE) {
			if (mmap[i].addr == (uint64_t) bitmap) {
				uint64_t bitmap_end_byte = (uint64_t) bitmap + bitmap_bytes;
				uint64_t bitmap_end_page = ((bitmap_end_byte + 0x1000 - 1) / 0x1000) * 0x1000;
				uint64_t entry_end_page = (mmap[i].addr + mmap[i].len) / 0x1000;

				for (uint64_t page = bitmap_end_page; page < entry_end_page; page++) {
					pmm_set_page_free(page);
				}
			} else {
				uint64_t page = mmap[i].addr / 0x1000;
				uint64_t count = mmap[i].len / 0x1000;

				for (uint64_t j = 0; j < count; j++) {
					pmm_set_page_free(page + j);
				}
			}
		}
	}
}

uint8_t is_page_used(uint64_t page)
{
	uint64_t byte = page / 8;
	uint64_t bit  = page % 8;
	return (bitmap[byte] & (1<<bit)) >> bit;
}

uint64_t pmm_find_free_pages(uint64_t size)
{
	uint64_t needed_pages = (size + 0x1000 - 1) / 0x1000;
	uint64_t found_pages = 0;
	uint64_t current_page = 0;

	for (uint64_t i = 0; i < memory_pages; i++) {
		if(!is_page_used(i)) {
			if (found_pages == 0) {
				current_page = i;
			}
			found_pages++;
		} else {
			found_pages = 0;
		}
		if (found_pages >= needed_pages) {
			return current_page;
		}
	}

	serial_print("Ei löytynyt vapaata muistia!\n");
	while (1) {
		asm volatile("hlt");
	}
}

const uint16_t digits[] = {
    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9', u'a', u'b', u'c', u'd', u'e', u'f'
};

void printHexDigit(uint32_t n) {
    char buffer[9];
    char *p = buffer;

    for (int i = 7; i >= 0; --i) {
   uint8_t nibble = (n & (0xf << (i*4))) >> (i*4);
   *p++ = digits[nibble];
    }
    *p = 0;
    serial_print(buffer);
    serial_print("\n");
}

uint64_t *pmm_alloc(uint64_t size)
{
	uint64_t needed_pages = (size + 0x1000 - 1) / 0x1000;
	uint64_t free_page = pmm_find_free_pages(size);

	for (uint64_t i = 0; i < needed_pages; i++) {
		pmm_set_page_used(free_page + i);
	}

	serial_print("address = ");
	printHexDigit((uint32_t)bitmap+free_page);
	

	return (uint64_t*)bitmap+free_page;
}

void pmm_unalloc(uint64_t *addr, uint64_t size)
{
	char number[50];

	itoa(*(uint64_t*)addr, number, 10);
	serial_print("addr = ");
	serial_print(number);
	serial_print("\n");

	itoa(bitmap, number, 10);
	serial_print("bitmap = ");
	serial_print(number);
	serial_print("\n");
	uint64_t page = (uint64_t) *(uint64_t*)addr / 0x1000;
	uint64_t pages = (size + 0x1000 - 1) / 0x1000;

	for (uint64_t i = 0; i < pages; i++) {
		pmm_set_page_free(page + i);
	}
}
