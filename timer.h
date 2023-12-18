#ifndef __TIMER_H__
#define __TIMER_H__
#include "clint.h"
#include "types.h"

#define MAX_NUM_TIMER 10

typedef struct timer {
    void (*handler)(void *args);
    void *args;
    uint32_t timeout_tick;
} timer_t;

void init_timer(void);
timer_t *timer_create(
    void (*handler)(void *args),
    void *args,
    uint32_t timeout_tick
);
void timer_delete(timer_t *timer);
timer_t* get_timer_list();

#endif  /* __TIMER_H__ */
