#include "loadkern.h"
#include "linux/xz.h"

#define MAX_ELF_SIZE (64 * 1024 * 1024)

extern uint8_t MEMORY_BASE;
extern void *get_kernel(void);
extern uint32_t get_kernel_size(void);

void *loadkern(void)
{
	xz_crc32_init();
	struct xz_dec *dec = xz_dec_init(XZ_SINGLE, 0);
	if (dec == NULL)
		return NULL;
	struct xz_buf b;
	b.in = (uint8_t *) get_kernel();
	b.in_pos = 0;
	b.in_size = get_kernel_size();
	b.out = &MEMORY_BASE;
	b.out_pos = 0;
	b.out_size = MAX_ELF_SIZE;
	enum xz_ret r = xz_dec_run(dec, &b);
	if (r != XZ_STREAM_END)
		return NULL;
	xz_dec_end(dec);
	return &MEMORY_BASE;
}
