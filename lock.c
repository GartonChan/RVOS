#include "lock.h"
#include "uart.h"

#ifdef CLOSE_INT_ENABLE
static void acquire_spin_lock()
{
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
}

static void release_spin_lock()
{
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}

#else
void init_spin_lock(spin_lock_t *lock)
{
    lock->locked = 0;
}

void acquire_spin_lock(spin_lock_t *lock)
{
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0);
}

void release_spin_lock(spin_lock_t *lock)
{
    if (lock->locked == 0) {
        return ;
    } else if (lock->locked == 1) {
        __sync_lock_test_and_set(&lock->locked, 0);
    } else {
        uart_puts("WARN:Try to release a spin-lock with an unusual locked-value\n");
    }
}

#endif

