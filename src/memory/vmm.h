#ifndef VMM_H
#define VMM_H
#include <stdint.h>

#define MEM_PHYS_OFFSET ((size_t)0xffffffff80000000)

#define PML4_INDEX(virt) ((((uint64_t)(virt)) >> 39) & 511)
#define PML3_INDEX(virt) ((((uint64_t)(virt)) >> 30) & 511)
#define PML2_INDEX(virt) ((((uint64_t)(virt)) >> 21) & 511)
#define PML1_INDEX(virt) ((((uint64_t)(virt)) >> 12) & 511)

#define PAGE_PRESENT    (1ull << 0)
#define PAGE_WRITABLE   (1ull << 1)
#define PAGE_USER       (1ull << 2)
#define PAGE_WRITETHRU  (1ull << 3)
#define PAGE_NOCACHE    (1ull << 4)
#define PAGE_ACCESSED   (1ull << 5)
#define PAGE_DIRTY      (1ull << 6)
#define PAGE_HUGE       (1ull << 7)
#define PAGE_GLOBAL     (1ull << 8)
#define PAGE_NX         (1ull << 63)

void init_vmm();

#endif
