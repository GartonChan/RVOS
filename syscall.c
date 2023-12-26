#include "riscv.h"
#include "syscall.h"
#include "uart.h"

static uint32_t sys_gethid(uint32_t *hid)
{
    printf("--> sys_gethid, arg0=0x%x\n", hid);
    if (hid == NULL) {
        return -1U;
    }
    *hid = r_mhartid();
    return *hid;
}

/* Syscall Handler */
void syscall_handler(struct context *ctx)
{
    uart_puts("Handling the syscall...\n");
    uint32_t syscall_num = ctx->a7;
    switch (syscall_num) {
    case SYS_gethid:
        uart_puts("SYS_gethid\n");
        ctx->a0 = sys_gethid((uint32_t *)&(ctx->a0));
        break;
    default:
        printf("Unknown syscall no: %u\n", syscall_num);
        ctx->a0 = -1U;
    }
}
