#include "platform.h"

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

void timer_load(int interval);
void timer_init(void);
