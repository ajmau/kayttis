#ifndef PMM_H
#define PMM_H

#include "../stivale.h"

void init_pmm(stivale_info_t*);
uint64_t *pmm_alloc(uint64_t size);
void pmm_unalloc(void* addr, uint64_t size);

#endif
