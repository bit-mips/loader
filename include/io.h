#ifndef _io_h_
#define _io_h_

#include <stdint.h>

extern void io_init(void);

extern void outc(char c);
extern char getc(void);

extern void outs(char *s);
extern void outb(uint8_t b);
extern void outh(uint16_t h);
extern void outw(uint32_t w);

#endif
