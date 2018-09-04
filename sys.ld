ENTRY(_start)

/* MEMORY_BASE = 0x80000000; */
MEMORY_TOP = 0x88000000;

UART_BUF = 0xb0001000;
UART_DLL = 0xb0001000;
UART_DLM = 0xb0001004;
UART_LCR = 0xb000100c;
UART_LSR = 0xb0001014;

SECTIONS
{
	. = 0x80000000;
	.data : {
		*(.data*)
		*(.bss*)
		*(.sbss*)
	}
	MEMORY_BASE = .;
	. = 0x9fc00000;
	.text : {
		*(.init*)
		*(.text*)
		*(.rodata*)
	}
}
