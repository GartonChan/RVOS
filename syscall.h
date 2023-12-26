#ifndef __SYSCALL_H__
#define __SYSCALL_H__
#include "types.h"

/* System call numbers */
#define SYS_gethid 1  /* Just for demo */

void syscall_handler(struct context *ctx);

/* Defined in usys.S */
extern uint32_t gethid(uint32_t *hid);

#endif  /* __SYSCALL_H__ */
