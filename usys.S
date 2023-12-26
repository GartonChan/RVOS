/* User Syscall */

.global gethid
gethid:
	li a7, 1  # SYS_gethid
	ecall
	ret
