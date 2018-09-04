#define _STRING(x) #x
#define STRING(x) _STRING(x)
.text
.global get_kernel
get_kernel:
	la $v0, kernel_begin
	jr $ra

.global get_kernel_size
get_kernel_size:
	la $v0, kernel_begin
	la $v1, kernel_end
	sub $v0, $v1
	jr $ra

.section .rodata
kernel_begin:
.incbin STRING(KERNEL_FILE)
kernel_end:
