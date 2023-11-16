#ifndef __OS_H__
#define __OS_H__

#include <stddef.h>
#include <stdarg.h>

#include "platform.h"
#include "type.h"

/* UART */
extern void uart_init(void);
extern void uart_puts(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);
/* other */

#endif  /* __OS_H__ */