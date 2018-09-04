#include "util.h"

extern uint8_t MEMORY_BASE;
extern uint8_t MEMORY_TOP;

static uint32_t rb = 0;

void meminit(void)
{
	rb = (uint32_t) &MEMORY_BASE / 2 + (uint32_t) &MEMORY_TOP / 2;
}

void *memreserve(size_t s)
{
	rb += 16 - rb % 16;
	void *r = (void *) rb;
	rb += s;
	return r;
}

int memequ(uint8_t *a, uint8_t *b, size_t s)
{
	for (; s > 0; s--)
		if (*(a++) != *(b++))
			return 0;
	return 1;
}

void zerofill(uint8_t *p, size_t s)
{
	for (; s > 0; s--)
		*(p++) = 0;
}

static inline void *aligned4_memmove(uint32_t *a, uint32_t *b, size_t n)
{
	n /= 4;
	int incr;
	uint32_t *pa, *pb;
	if (a > b) {
		incr = -1;
		pa = a + n;
		pb = b + n;
	} else {
		incr = 1;
		pa = a;
		pb = b;
	}
	for (; n > 0; n--) {
		*pa = *pb;
		pa += incr;
		pb += incr;
	}
	return a;
}

void *c_memmove(uint8_t *a, uint8_t *b, size_t n)
{
	if ((uint32_t) a % 4 == 0 && (uint32_t) b % 4 == 0 && n % 4 == 0)
		return aligned4_memmove(a, b, n);
	int incr;
	uint8_t *pa, *pb;
	if (a > b) {
		incr = -1;
		pa = a + n;
		pb = b + n;
	} else {
		incr = 1;
		pa = a;
		pb = b;
	}
	for (; n > 0; n--) {
		*pa = *pb;
		pa += incr;
		pb += incr;
	}
	return a;
}
