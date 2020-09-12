#include <stdint.h>
#include <stddef.h>
#include "pmm.h"
#include "vmm.h"
#include "../portio.h"

#define write_cr(reg, val) ({ \
    asm volatile ("mov cr" reg ", %0" : : "r" (val)); \
})

#define read_cr(reg) ({ \
    size_t cr; \
    asm volatile ("mov %0, cr" reg : "=r" (cr)); \
    cr; \
})

#define invlpg(addr) ({ \
    asm volatile ( \
        "invlpg [%0];" \
        : \
        : "r" (addr) \
    ); \
})

void write_cr3(uint64_t value) {
    __asm__("mov %0, %%cr3" : /* no output */ : "r"(value));
}

uint64_t read_cr3() {
    uint64_t value;

    __asm__("mov %%cr3, %0" : "=r"(value) : /* no input */);

    return value;
}

char * itoa2( int value, char * str, int base )
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


typedef uint64_t pt_entry_t;

void map_page(pt_entry_t *pml4, size_t phys_addr, size_t virt_addr, size_t flags)
{
	size_t pml4_entry = (virt_addr & ((size_t)0x1ff << 39)) >> 39;
	size_t pdpt_entry = (virt_addr & ((size_t)0x1ff << 30)) >> 30;
	size_t pd_entry = (virt_addr & ((size_t)0x1ff << 21)) >> 21;
	size_t pt_entry = (virt_addr & ((size_t)0x1ff << 12)) >> 12;

	pt_entry_t *pdpt, *pd, *pt;
	
	if (pml4[pml4_entry] & PAGE_PRESENT) {
		serial_print("pml4_entry is present\n");
		pdpt = (pt_entry_t*)((pml4[pml4_entry] & 0xfffffffffffff000)); //+ MEM_PHYS_OFFSET);
	} else {
		serial_print("alloc pdpt\n");
		pdpt = (pt_entry_t*)((size_t)pmm_alloc(1)); // + MEM_PHYS_OFFSET);

		pml4[pml4_entry] = (pt_entry_t)((size_t)pdpt - MEM_PHYS_OFFSET) | PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE; 
	}

	if (pdpt[pdpt_entry] & PAGE_PRESENT) {
		serial_print("pdpt_entry is present\n");
		pd = (pt_entry_t *)((pdpt[pdpt_entry] & 0xfffffffffffff000)); //+ MEM_PHYS_OFFSET);
	} else {
		serial_print("alloc pd\n");
		pd = (pt_entry_t*)((size_t)pmm_alloc(1) ); ///+ MEM_PHYS_OFFSET);

		pdpt[pdpt_entry] = (pt_entry_t)((size_t)pd - MEM_PHYS_OFFSET) | PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE;
	}

	if (pd[pt_entry] & PAGE_PRESENT) {
		serial_print("pt_entry is present\n");
		pt = (pt_entry_t *)((pd[pd_entry] & 0xfffffffffffff000)); //+ MEM_PHYS_OFFSET);
	} else {
		serial_print("alloc pt\n");
		pt = (pt_entry_t*)((size_t)pmm_alloc(1) ); ///+ MEM_PHYS_OFFSET);

		pd[pd_entry] = (pt_entry_t)((size_t)pt - MEM_PHYS_OFFSET) | PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE;
	}

	pt[pt_entry] = (pt_entry_t)(phys_addr | flags);
}
	
void init_vmm()
{
	serial_print("init_vmm()\n");

	char number[50];

	pt_entry_t *joo = pmm_alloc(1);

	serial_print("---------- looppi ---------- \n");
	for (size_t i = 0; i < (0x2000000 / 4096); i++) {
		serial_print("i on ");
		itoa2(i, number, 10);
		serial_print(number);
		serial_print("\n");
		size_t addr = i * 4096;
		map_page(joo, addr, addr, PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER);
		map_page(joo, 0x0, MEM_PHYS_OFFSET+addr, PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER);
	}
/*	serial_print("---------- looppi ---------- \n");
	for (size_t i = 0; i < (0xffff800000000000 / 4096); i++) {
		serial_print("i on ");
		itoa2(i, number, 10);
		serial_print(number);
		serial_print("\n");
		size_t addr = i * 4096;
		map_page(joo, addr, MEM_PHYS_OFFSET+addr, PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER);
	}*/


//	map_page(joo, 0x3c0000000000, 0x3c0000000000, PAGE_WRITABLE|PAGE_PRESENT|PAGE_USER);
	//write_cr3((uint64_t)joo);

	write_cr3((uint64_t)joo);
}
