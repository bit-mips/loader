.section .init
_entry:
	la $t0, _start
	jr $t0

.text
.global _start
.global MEMORY_TOP
_start:
	mfc0 $8, $12
	and $t0, 0xffc7ff00
	mtc0 $8, $12
	la $sp, MEMORY_TOP
	j main

.set noat
.set noreorder
.section .init
#define EXC_HANDLER \
	la $k0, MEMORY_TOP; \
	sw $ra, -4($k0); \
	la $k0, exc_storeframe; \
	jalr $k0
.org 0x380
	EXC_HANDLER;
.org 0x400
	EXC_HANDLER;

.text
exc_storeframe:
	la $k0, MEMORY_TOP
	sub $k0, 4
#define STOREREG(n) \
	sub $k0, 4; \
	sw $n, 0($k0)
	STOREREG(30);
	STOREREG(29);
	STOREREG(28);
	STOREREG(27);
	STOREREG(26);
	STOREREG(25);
	STOREREG(24);
	STOREREG(23);
	STOREREG(22);
	STOREREG(21);
	STOREREG(20);
	STOREREG(19);
	STOREREG(18);
	STOREREG(17);
	STOREREG(16);
	STOREREG(15);
	STOREREG(14);
	STOREREG(13);
	STOREREG(12);
	STOREREG(11);
	STOREREG(10);
	STOREREG(9);
	STOREREG(8);
	STOREREG(7);
	STOREREG(6);
	STOREREG(5);
	STOREREG(4);
	STOREREG(3);
	STOREREG(2);
	STOREREG(1);
	STOREREG(0);
	move $sp, $k0
	move $a1, $k0
	sub $a0, $ra, 28
	jal trap_entry
