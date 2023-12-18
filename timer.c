#include "timer.h"
#include "uart.h"

static timer_t sw_timer[MAX_NUM_TIMER];

void init_timer(void)
{
    for (int i = 0; i < MAX_NUM_TIMER; i++) {
        sw_timer[i].handler = NULL;
        sw_timer[i].args = NULL;
        sw_timer[i].timeout_tick = 0;
    }
}

timer_t *timer_create(
    void (*handler)(void *args),
    void *args,
    uint32_t timeout_tick)
{
    uint32_t _tick = get_timer_tick();
    timer_t *timer = &(sw_timer[0]);
    
    /* allocate if available */
    int i;
    for (i = 0; i < MAX_NUM_TIMER; i++) {
        if (timer->handler == NULL) {
            break;
        }
        timer++;
    }
    if (i >= MAX_NUM_TIMER) {
        uart_puts("Failed to create timer.\n");
        return NULL;
    }
    /* timer setting */
    timer->handler = handler;
    timer->args = args;
    timer->timeout_tick = _tick + timeout_tick;

    return timer;
}

void timer_delete(timer_t *timer)
{
    if (!timer) {
        uart_puts("timer_delete: NULL ptr\n");
        return ;
    }
    for (int i = 0; i < MAX_NUM_TIMER; i++) {
        if (&(sw_timer[i]) == timer) {
            printf("deleted %d-th software timer\n", i);
            /* DO NOT write to pointer without checking */
            timer->handler = NULL;
            timer->args = NULL;
            timer->timeout_tick = 0;
            break;
        }
    }
}

timer_t* get_timer_list()
{
    return &(sw_timer[0]);
}