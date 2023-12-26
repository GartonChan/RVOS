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
	/* x0 is hardwired with all bits equal to 0. */
	sw ra, 0(\base)     /* x1 */
	sw sp, 4(\base)     /* x2 */
	sw gp, 8(\base)     /* x3 */
	sw tp, 12(\base)    /* x4 */
	sw t0, 16(\base)    /* x5 */
	sw t1, 20(\base)    /* x6 */
	sw t2, 24(\base)    /* x7 */
	sw s0, 28(\base)    /* x8 */
	sw s1, 32(\base)    /* x9 */
	sw a0, 36(\base)    /* x10 */
	sw a1, 40(\base)    /* x11 */
	sw a2, 44(\base)    /* x12 */
	sw a3, 48(\base)    /* x13 */
	sw a4, 52(\base)    /* x14 */
	sw a5, 56(\base)    /* x15 */
	sw a6, 60(\base)    /* x16 */
	sw a7, 64(\base)    /* x17 */
	sw s2, 68(\base)    /* x18 */
	sw s3, 72(\base)    /* x19 */
	sw s4, 76(\base)    /* x20 */
	sw s5, 80(\base)    /* x21 */
	sw s6, 84(\base)    /* x22 */
	sw s7, 88(\base)    /* x23 */
	sw s8, 92(\base)    /* x24 */
	sw s9, 96(\base)    /* x25 */
	sw s10, 100(\base)  /* x26 */
	sw s11, 104(\base)  /* x27 */
	sw t3, 108(\base)   /* x28 */
	sw t4, 112(\base)   /* x29 */
	sw t5, 116(\base)   /* x30 */
	# sw t6, 120(\base)   /* x31 */
	# we don't save t6 here, due to we have used it
	# as base, we have to save t6 in an extra step
	# outside of reg_save
.endm


# restore all GP registers from ctx except gp & tp
# struct context *base = &ctx_task;
# ra = base->ra;
# ......
.macro reg_restore base
	/* x0 is hardwired with all bits equal to 0. */
	lw ra, 0(\base)     /* x1 */
	lw sp, 4(\base)     /* x2 */
	lw gp, 8(\base)     /* x3 */
	lw tp, 12(\base)    /* x4 */
	lw t0, 16(\base)    /* x5 */
	lw t1, 20(\base)    /* x6 */
	lw t2, 24(\base)    /* x7 */
	lw s0, 28(\base)    /* x8 */
	lw s1, 32(\base)    /* x9 */
	lw a0, 36(\base)    /* x10 */
	lw a1, 40(\base)    /* x11 */
	lw a2, 44(\base)    /* x12 */
	lw a3, 48(\base)    /* x13 */
	lw a4, 52(\base)    /* x14 */
	lw a5, 56(\base)    /* x15 */
	lw a6, 60(\base)    /* x16 */
	lw a7, 64(\base)    /* x17 */
	lw s2, 68(\base)    /* x18 */
	lw s3, 72(\base)    /* x19 */
	lw s4, 76(\base)    /* x20 */
	lw s5, 80(\base)    /* x21 */
	lw s6, 84(\base)    /* x22 */
	lw s7, 88(\base)    /* x23 */
	lw s8, 92(\base)    /* x24 */
	lw s9, 96(\base)    /* x25 */
	lw s10, 100(\base)  /* x26 */
	lw s11, 104(\base)  /* x27 */
	lw t3, 108(\base)   /* x28 */
	lw t4, 112(\base)   /* x29 */
	lw t5, 116(\base)   /* x30 */
	lw t6, 120(\base)   /* x31 */
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
# ----- comment the process of save ctx here for preemptive -----
#     # t6: as the 'base' for reg_save/reg_restore (as ctx pointer)
#     csrrw t6, mscratch, t6  # swap t6 and mscratch(t6 <-> mscratch)
    
# 	# Note: if mscratch is initialized as zero in sched_init(), which
# 	# makes t6 zero,the first time switch_to() is called, and that is
# 	# the special case we have to handle with t6.
# 	# beqz t6, 1f  # label-name: 1 (forward), patch: comment this line

#     reg_save t6  # save ctx of previous task (mscratch)

#     # Save the actual t6 register, which we swapped into mscratch
#     # regs without t6 so we need to save t6 in an extra step
#     mv t5, t6
#     csrr t6, mscratch  # t5 also points to the ctx of current task
#     sw t6, 120(t5)  # save t6(current ctx pointer) with t5 as base

# 1:
    # switch mscratch to point to the ctx of the next task
    csrw mscratch, a0  # pass the next task ctx via a0 (*next).
    
	# restore the pc in the next schedule cycle.
	lw a1, 124(a0)
	csrw mepc, a1

    # Restore all GP registers
    mv t6, a0  # using t6 as 'base' to reg_restore
    reg_restore t6 

    # Do actual ctx switching (to next task).
    # ret  # return to ra(return address) 
	
	# use mret for preemptive schedule
	mret

.global trap_vector
.align 4
trap_vector:
	# save the current ctx(pointed by mscratch)
	# Save all GP-Registers 
	csrrw t6, mscratch, t6  # swap t6 and mscratch
	reg_save t6  # using t6 as 'base' to reg_save

	# save the actual t6, which is swapped into mscratch
	mv t5, t6  # makest5 be 'base' for storing t6 and pc
	
	csrr t6, mscratch  # read t6 back from mscratch
	sw t6, 120(t5)  # save t6 using t5 as 'base'
	
	csrr a0, mepc  # read mepc into a0
	sw a0, 124(t5)  # save pc using t5 as 'base'

	# restore the ctx ptr into mscratch
	csrw mscratch, t5

	# call the C trap_handler in trap.c
	csrr a0, mepc
	csrr a1, mcause
	csrr a2, mscratch  # ctx pointer
	call trap_handler

	# trap_handler will return via a0
	csrw mepc, a0  # a0: return_pc

	# restore ctx
	csrr t6, mscratch
	reg_restore t6
	
	# return to whatever we were doing before trap
	mret

.end