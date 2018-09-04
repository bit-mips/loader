#define C0_Count $9

.text
.global clearcount
clearcount:
	mtc0 $zero, C0_Count
	jr $ra

.global readcount
readcount:
	mfc0 $v0, C0_Count
	jr $ra

.global jumpto
jumpto:
	jr $a0

#define GEN_READ_C0(name, no) \
.global read_c0_ ## name; \
read_c0_ ## name: \
	mfc0 $v0, $no; \
	jr $ra

GEN_READ_C0(badvaddr, 0);
GEN_READ_C0(status, 12);
GEN_READ_C0(cause, 13);
GEN_READ_C0(epc, 14);
