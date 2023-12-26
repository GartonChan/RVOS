#include "platform.h"
#include "riscv.h"
#include "plic.h"
#include "types.h"
#include "uart.h"
#include "clint.h"
#include "timer.h"
#include "syscall.h"

extern void trap_vector(void);
extern void schedule(void);

void trap_init()
{
    /* Set the trap-vector base-address for machine mode */
    w_mtvec((reg_t)trap_vector);  /* Write trap_vector into mtvec */
    w_mie(r_mie() | MIE_MTIE);    /* Timer */
    w_mie(r_mie() | MIE_MSIE);    /* Software */
    w_mie(r_mie() | MIE_MEIE);    /* External */
    w_mstatus(r_mstatus() | MSTATUS_MIE);  /* Global Interrupt */
}

/* m-mode external_interrupt, mcause is 0x8000000B */
static void external_interrupt_handler(void)
{
    int irq = plic_claim();
    
    if (irq == UART0_IRQ) {
        uart_puts("This key raised external interrupt: ");
        uart_irq();
    } else if (irq) {
        printf("Unexpected interrupt irq = %d\n", irq);
    }

    if (irq) {
        plic_complete(irq);
    }
}

static void timer_check()
{
    uint32_t _tick = get_timer_tick();
    timer_t *timer = get_timer_list();
    for(int i = 0; i < MAX_NUM_TIMER; i++) {
        if (timer->handler != NULL){
            if (timer->timeout_tick <= _tick){
                timer->handler(timer->args);
                // add a new field to identify single time or cycle
                timer->handler = NULL;
                timer->args = NULL;
                timer->timeout_tick = 0;
            }
        }
        timer++;
    }
}

/* m-mode timer_interrupt, mcause is 0x80000007 */
static void timer_interrupt_handler(void)
{
    /*  Notice to load timer first. */
    timer_load(TIMER_INTERVAL / 10);  /* 100ms interval */
    update_tick();
    uint32_t _tick = get_timer_tick();   
    // something to do in timer_interrupt...
    printf("Handle the timer interrupt, _tick = %d\n", _tick);
    timer_check();
    schedule();
}

/* m-mode software_interrupt, mcause is 0x80000003 */
static void software_interrupt_handler(void)
{
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 0;
    schedule();
}

reg_t trap_handler(reg_t mepc, reg_t cause, struct context *ctx)
{
    reg_t return_pc = mepc;
    reg_t cause_code = cause & 0xfff;
    if (cause & 0x80000000) {
        /* Asynchronous trap - interrupt */
        printf("Interrupt, cause_code = %d\n", cause_code);
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n");
            software_interrupt_handler();
            break;
        case 7:
            uart_puts("timer_interruption!\n");
            timer_interrupt_handler();
            break;
        case 11:
            uart_puts("external interruption!\n");
            external_interrupt_handler();
            break;
        default:
            uart_puts("unknown async exception!\n");
            break;
        }
    } else {
        /* Synchronous trap - exception */
        printf("Exception, cause_code = %d\n", cause_code);
        switch (cause_code) {
        case 8:
            uart_puts("Ecall from U-mode\n");
            /* Handle the Syscall */
            syscall_handler(ctx);
            return_pc += 4;
            break;
        case 9:
            uart_puts("Ecall from S-mode\n");
            return_pc += 4;
            break;
        case 11:
            uart_puts("Ecall from M-mode\n");
            return_pc += 4;
            break;
        default:
            panic("unknown sync exception!\n");
            break;
        }
    }
    return return_pc;
}

void trap_test(void)
{
    /* 
     * Synchronous exception code = 7 
     * Store/AMO access fault
     */
    *(int *)0x00000000 = 100;

    /*
     * Synchronous exception code = 5
     * Load access fault
     */
    // int a = *(int *)0x00000000;

    uart_puts("Yeah! I am back from trap!\n");
}