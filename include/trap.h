#ifndef _trap_h_
#define _trap_h_

#include <stdint.h>

struct trap_frame {
	uint32_t regs[32];
};

extern void trap_entry(void *excentry, struct trap_frame *tf);

#endif
