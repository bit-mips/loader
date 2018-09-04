#include "io.h"
#include <stdint.h>

#define CLK_UART 50000000
#define UART_BAUD 115200
#define UART_DIV ((CLK_UART) / (16 * (UART_BAUD)))

extern volatile uint8_t UART_BUF;
extern volatile uint8_t UART_DLL;
extern volatile uint8_t UART_DLM;
extern volatile uint8_t UART_LCR;
extern volatile uint8_t UART_LSR;

void io_init(void)
{
	uint16_t div = UART_DIV;
	UART_LCR = 0x80;
	UART_DLL = div & 0xff;
	UART_DLM = div >> 8;
	UART_LCR = 0x03;
}

void outc(char c)
{
	while (!(UART_LSR & 0x40)) {};
	UART_BUF = (uint8_t) c;
}

char getc(void)
{
	while (!(UART_LSR & 0x01)) {};
	return (char) UART_BUF;
}

void outs(char *s)
{
	while (*s)
		outc(*(s++));
}

static void outhb(uint8_t hb)
{
	switch (hb) {
#define CASE(n) case 0x ## n : \
		outc(* #n); \
		break;
		CASE(0);
		CASE(1);
		CASE(2);
		CASE(3);
		CASE(4);
		CASE(5);
		CASE(6);
		CASE(7);
		CASE(8);
		CASE(9);
		CASE(a);
		CASE(b);
		CASE(c);
		CASE(d);
		CASE(e);
		CASE(f);
#undef CASE
	}
}

void outb(uint8_t b)
{
	outhb(b >> 4);
	outhb(b & 0x0f);
}

void outh(uint16_t h)
{
	outb(h >> 8);
	outb(h & 0x00ff);
}

void outw(uint32_t w)
{
	outh(w >> 16);
	outh(w & 0x0000ffff);
}
