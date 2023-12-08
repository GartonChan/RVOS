#ifndef __OS_H__
#define __OS_H__

#include "platform.h"
#include "types.h"

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
extern void timer_load(int interval);

#endif  /* __OS_H__ */