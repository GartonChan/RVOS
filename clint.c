#include "riscv.h"
#include "types.h"
#include "clint.h"

static uint32_t _tick = 0;

void timer_load(uint64_t interval)
{
    int hart_id = r_mhartid();
    *(uint64_t *)CLINT_MTIMECMP(hart_id) = 
            *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init(void)
{
    timer_load(TIMER_INTERVAL);
    
    /* Enable machine-mode timer interrupts */
    w_mie(r_mie() | MIE_MTIE);

    /* Enable machine-mode global interrupts */
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}

uint32_t get_timer_tick()
{
    return _tick;
}

void update_tick()
{
    _tick += 1;
}
