/* User Syscall */

.global gethid
gethid:
	li a7, 1  # SYS_gethid
	ecall
	lw a0, (a0) 
	ret
