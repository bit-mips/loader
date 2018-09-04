#ifndef _util_h_
#define _util_h_

#include <stddef.h>
#include <stdint.h>

extern void clearcount(void);
extern uint32_t readcount(void);
extern void jumpto(void *p);
extern uint32_t read_c0_badvaddr(void);
extern uint32_t read_c0_status(void);
extern uint32_t read_c0_cause(void);
extern uint32_t read_c0_epc(void);

extern void meminit(void);
extern void *memreserve(size_t s);
extern int memequ(uint8_t *a, uint8_t *b, size_t s);
extern void zerofill(uint8_t *p, size_t s);
extern void *c_memmove(uint8_t *a, uint8_t *b, size_t n);

#endif
