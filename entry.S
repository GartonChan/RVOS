# Save all General-Purpose(GP) registers to context.
# struct context *base = &ctx_task;
# base->ra = ra;
# ......
# 
# These GP registers to be saved DO NOT include gp
# and tp, because they are not caller-saved or
# callee-saved.
# 
# These two register are often used for special
# purpose. For example, in RVOS, 'tp'(aka "thread
# pointer") is used to store hartid, which is a
# global value and would not be changed during
# context-switch.

.macro reg_save base
	sw ra, 0(\base)
	sw sp, 4(\base)
	sw t0, 16(\base)
	sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
	# we don't save t6 here, due to we have used it
	# as base, we have to save t6 in an extra step
	# outside of reg_save
.endm


# restore all GP registers from ctx except gp & tp
# struct context *base = &ctx_task;
# ra = base->ra;
# ......
.macro reg_restore base
	lw ra, 0(\base)
	lw sp, 4(\base)
	lw t0, 16(\base)
	lw t1, 20(\base)
	lw t2, 24(\base)
	lw s0, 28(\base)
	lw s1, 32(\base)
	lw a0, 36(\base)
	lw a1, 40(\base)
	lw a2, 44(\base)
	lw a3, 48(\base)
	lw a4, 52(\base)
	lw a5, 56(\base)
	lw a6, 60(\base)
	lw a7, 64(\base)
	lw s2, 68(\base)
	lw s3, 72(\base)
	lw s4, 76(\base)
	lw s5, 80(\base)
	lw s6, 84(\base)
	lw s7, 88(\base)
	lw s8, 92(\base)
	lw s9, 96(\base)
	lw s10, 100(\base)
	lw s11, 104(\base)
	lw t3, 108(\base)
	lw t4, 112(\base)
	lw t5, 116(\base)
	lw t6, 120(\base)
.endm

# *********** Something to note about save/restore **************
# - We use 'mscratch' to hold a pointer to context of current task
# - We use 't6' as the 'base' for reg_save/reg_restore, because it 
#   is the very bottom register (x31) and would not be overwritten
#   during loading.
# - Note: CSRs(mscratch) can not be used as 'base' due to load/restore
#   instruction only accept general purpose registers.

.text

# def void switch_to(struct context *next) here;
# a0: pointer to ctx of the next task
# mscratch: point to ctx of the current task
.global switch_to
.align 4
switch_to:
    # t6: as the 'base' for reg_save/reg_restore (as ctx pointer)
    csrrw t6, mscratch, t6  # swap t6 and mscratch(t6 <-> mscratch)
    
	# Note: the first time switch_to() is called, mscratch is
	# initialized as zero (in sched_init()), which makes t6 zero,
	# and that is the special case we have to handle with t6.
	beqz t6, 1f  # label-name: 1 (forward)
	
    reg_save t6  # save ctx of previous task (mscratch)

    # Save the actual t6 register, which we swapped into mscratch
    # regs without t6 so we need to save t6 in an extra step
    mv t5, t6
    csrr t6, mscratch  # t5 also points to the ctx of current task
    sw t6, 120(t5)  # save t6(current ctx pointer) with t5 as base

1:
    # switch mscratch to point to the ctx of the next task
    csrw mscratch, a0  # a0: pointer to ctx of new task
    
    # Restore all GP registers
    # Use t6 to point to the ctx of the new task
    mv t6, a0  # using t6 as 'base' to reg_restore
    reg_restore t6

    # Do actual ctx switching (to next task).
    ret  # return to ra(return address)

.global trap_vector
.align 4
trap_vector:
	# save the current ctx(pointed by mscratch)
	csrrw t6, mscratch, t6  # swap t6 and mscratch
	beqz t6, 2f  # label-name: 2 (forward)
	reg_save t6  # Why does it iterate here? -> infinite trap

	# save the actual t6, swapped into mscratch
	mv t5, t6  # make t5 also points to the current ctx
	csrr t6, mscratch  # read t6 back from mscratch
	sw t6, 120(t5)  # save t6 with t5 as base (manually save)

	# restore the ctx ptr into mscratch
	csrw mscratch, t5
2:
	# call the C trap_handler in trap.c
	csrr a0, mepc
	csrr a1, mcause
	call trap_handler

	# trap_handler will return epc via a0
	csrw mepc, a0  # a0: return address

	# restore ctx
	csrr t6, mscratch
	reg_restore t6

	# return to whatever we were doing before trap
	mret

.end