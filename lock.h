#ifndef __LOCK_H__
#define __LOCK_H__

#include "platform.h"
#include "riscv.h"

// #define CLOSE_INT_ENABLE`

#ifdef CLOSE_INT_ENALBE
static void acquire_spin_lock();
static void release_spin_lock();

#else
typedef struct spin_lock {
    int locked;
}spin_lock_t;

void init_spin_lock(spin_lock_t *spin_lock);
void acquire_spin_lock(spin_lock_t *spin_lock);
void release_spin_lock(spin_lock_t *spin_lock);

#endif
#endif /* __LOCK_H__ */