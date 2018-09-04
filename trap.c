#include "trap.h"
#include "io.h"
#include "util.h"
#include <stdint.h>

void trap_entry(void *excentry, struct trap_frame *tf)
{
	outs("BIT-MIPS firmware exception handler at 0x");
	outw((uint32_t) excentry);
	outs("\r\nfatal error: unexpected exception thrown from 0x");
	outw(read_c0_epc());
	outs(",\r\n    cp0_badvaddr = 0x");
	outw(read_c0_badvaddr());
	outs(",\r\n    cp0_status = 0x");
	outw(read_c0_status());
	outs(",\r\n    cp0_cause = 0x");
	uint32_t cause = read_c0_cause();
	outw(cause);
	outs(", exccode = 0x");
	outb((cause >> 2) & 0x1f);
	outs(".\r\nregisters captured on trap: (k0 scratched out)\r\n");
	int i;
	for (i = 0; i < 32; i++) {
		outs("    reg 0x");
		outb(i);
		outs(" = 0x");
		outw(tf->regs[i]);
		outs("\r\n");
	}
	outs("entering dead loop.\r\n");
	for (;;);
}
