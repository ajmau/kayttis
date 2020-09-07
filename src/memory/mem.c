#include <stdint.h>

void memset(uint64_t *ptr, int value, int size)
{
	for (uint64_t i=0; i <= size; i++) {
		ptr[i] = value;
	}
}
