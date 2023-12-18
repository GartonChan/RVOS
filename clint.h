#ifndef __CLINC_H__
#define __CLINC_H__

#include "platform.h"
#include "types.h"

/* cycles since boot(initial value: 0), 
 * 64-bit width ignoring the platform */
#define CLINT_MTIME (CLINT_BASE + 0xBFF8)  

/* Each hart has a mtimecmp register,
 * without default initial value
 * also the same width as 64-bit */
#define CLINT_MTIMECMP(hart) (CLINT_BASE +  0x4000 + (hart)*8)

/* 10000000 ticks per-second */
#define CLINT_TIMEBASE_FREQ 10000000
/* Interval ~= 1 second */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

/* 
 * Each hart has a msip register, 32-bit width, only
 * bit-0 will be used, which is mapped to mip.MSIP.
 * Setting bit-0 as 1 to raise software interrupt.
 * Setting bit-0 as 0 to clear the pending bit after
 *     handling a software interrupt.   
 */
#define CLINT_MSIP(hart) (CLINT_BASE + 4 * (hart))


void timer_init(void);
void timer_load(uint64_t interval);
uint32_t get_timer_tick();
void update_tick();

#endif /* __CLINC_H__ */
