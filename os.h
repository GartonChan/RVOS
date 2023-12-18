#ifndef __OS_H__
#define __OS_H__

#include "platform.h"
#include "types.h"
#include "timer.h"

/* Here are the interfaces provided by OS */

/* UART */
extern int uart_putc(char ch);
extern void uart_puts(char *s);
extern int uart_getc(void);

/* printf*/
extern int printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);

/* multitask */
extern int task_create(void (*start_routin)(void));
extern void task_yield();
extern void task_delay(volatile int count);

/* PLIC */
extern int plic_claim(void);
extern void plic_complete(int irq);

/* CLINT */
extern void timer_load(uint64_t interval);

/* Software Timer */
extern void init_timer(void);
extern timer_t *timer_create(
    void (*handler)(void *args),
    void *args,
    uint32_t timeout_tick
);
extern void timer_delete(timer_t *timer);

#endif  /* __OS_H__ */