#include "platform.h"
/* Act as a bootloader */
    # size of each hart's stack is 1024 bytes
    .equ STACK_SIZE, 1024  # 1 << 10
    .global _start
    .text

_start:
    # Park harts with id != 0
    csrr t0, mhartid
    mv tp, t0  # keep the hart's id in its tp for later usage.
    bnez t0, park  # if not hart with id0, make it park

    # Stack initialization (other harts will not go here)
    # setup stacks, the stack grows from bottom to top,
    # so we put the stack pointer to the very end of the stack range.
    slli t0, t0, 10  # t0 << 10(size of stack space);
    la sp, stacks + STACK_SIZE  # set the initial sp to the
                                # end of the first stack space.
    add sp, sp, t0  # move the current hart sp to its place
                    # in the stack space(increase from top to bottom).

    # Syscall
    #ifdef CONFIG_SYSCALL
    # https://lore.kernel.org/qemu-devel/20201223192553.332508-1-atish.patra@wdc.com/
	# For qemu version >= 6.0, exception would be raised if no PMP enty is
	# configured. So just configure one entny, which allows all the whole
	# 32-bits physical address range is R/W/X.
	# FIXME: I say it is a temporary workaroud due to I think the patch
	# above contains bug, and I have raised new issue to qemu but it has not
	# been rootcaused till now. Details please refer to
	# https://gitlab.com/qemu-project/qemu/-/issues/585 or
	# https://gitee.com/unicornx/riscv-operating-system-mooc/issues/I441IC (in chinese)
	# So it's just a temporary workaround till now to not block people who
	# want to try newer qemu (>= 6.0).
	li      t0, 0xffffffff
	csrw    pmpaddr0, t0
	li      t0, 0xf
	csrw    pmpcfg0, t0
    #endif 

    #ifdef CONFIG_SYSCALL
    # At the end of start_kernel, schedule() will call MRET to switch
	# to the first task, so we parepare the mstatus here.
	# Notice: default mstatus is 0
    li	t0, 0
    #else
    li t0, 1 << 7 | 1 << 13
    #endif
    csrr a1, mstatus
    or   t0, t0, a1
    csrw mstatus, a1

    j start_kernel  # The hart with id0 jump to start_kernel

park:
    wfi  # wait for interrupt (IPI), work in a low power mode
    j park  # loop

# In the standard RISC-V calling convention
# the stack pointer is always 16-byte slligned. 
.align 16
# The symbol "stacks" indicates to the start address of the stack space.
stacks:
    .skip STACK_SIZE * MAXNUM_CPU  # allocate stack space for all harts

.end  # End of file.