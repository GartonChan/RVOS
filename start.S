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