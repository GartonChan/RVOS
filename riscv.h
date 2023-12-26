#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

/* ---------- RISC-V Architure and Assembly ---------- */

/*
 * ---------- Context ----------
 * 32 GP-Registers: x0 ~ x31 
 * Append pc(epc) for preemitive
 */
struct context {
    /* ignore x0 */
    reg_t ra;
    reg_t sp;
    reg_t gp;
    reg_t tp;
    reg_t t0;
    reg_t t1;
    reg_t t2;
    reg_t s0;
    reg_t s1;
    reg_t a0;
    reg_t a1;
    reg_t a2;
    reg_t a3;
    reg_t a4;
    reg_t a5;
    reg_t a6;
    reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
    /* upon is trap frame*/

    /* to store the epc in next schedule cycle before the trap */
    reg_t pc;
};

/*
 * ref: https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/riscv.h
 */
static inline reg_t r_tp()
{
    reg_t x;
    asm volatile("mv %0, tp" : "=r" (x));
    return x;
}

/* To tell which hart is this */
static inline reg_t r_mhartid()
{
    reg_t x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
}

/* Machine Status Register, mstatus */
#define MSTATUS_MPP (3 << 11)
#define MSTATUS_SPP (1 << 8)

#define MSTATUS_MPIE (1 << 7) 
#define MSTATUS_SPIE (1 << 5)
#define MSTATUS_UPIE (1 << 4)

#define MSTATUS_MIE (1 << 3)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_UIE (1 << 0)

static inline reg_t r_mstatus()
{
    reg_t x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

static inline void w_mstatus(reg_t x)
{
    asm volatile("csrw mstatus, %0" : : "r" (x));
}

/* 
 * Machine exception program counter
 * holds the instruction address to 
 * which a return from exception will go 
 */
static inline void w_mepc(reg_t x)
{
    asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline reg_t r_mepc()
{
    reg_t x;
    asm volatile("csrr %0, mepc" : "=r" (x));
    return x;
}

/* Machine Scratch register for early trap handler */
static inline void w_mscratch(reg_t x)
{
    asm volatile("csrw mscratch, %0" : : "r" (x));
}

/* Machine-mode Trap Vector */
static inline void w_mtvec(reg_t x)
{
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

/* Machine-mode Interrupt Enable*/
#define MIE_MEIE (1 << 11) /* external */
#define MIE_MTIE (1 << 7)  /* timer */
#define MIE_MSIE (1 << 3)  /* software */

static inline reg_t r_mie()
{
    reg_t x;
    asm volatile("csrr %0, mie" : "=r" (x));
    return x;
}

static inline void w_mie(reg_t x)
{
    asm volatile("csrw mie, %0" : : "r" (x));
}

static inline reg_t r_mcause()
{
    reg_t x;
    asm volatile("csrr %0, mcause" : "=r" (x));
    return x;
}

#endif  /* __RISCV_H__ */
