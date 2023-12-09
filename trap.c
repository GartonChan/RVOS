#include "platform.h"
#include "riscv.h"
#include "plic.h"
#include "types.h"
#include "uart.h"
#include "clint.h"

extern void trap_vector(void);
extern void schedule(void);

void trap_init()
{
    /* Set the trap-vector base-address for machine mode */
    w_mtvec((reg_t)trap_vector);  /* Write trap_vector into mtvec */
    w_mie(r_mie() | MIE_MTIE);  /* Timer */
    w_mie(r_mie() | MIE_MSIE);  /* Software */
    w_mie(r_mie() | MIE_MEIE);  /* External */
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

static uint32_t _tick = 0;

/* m-mode timer_interrupt, mcause is 0x80000007 */
static void timer_interrupt_handler(void)
{
    /*  Notice to load timer first. */
    timer_load(TIMER_INTERVAL);

    // something to do in timer_interrupt...
    _tick++;
    printf("Handle the timer interrupt, _tick = %d\n", _tick);
    schedule();
}

/* m-mode software_interrupt, mcause is 0x80000003 */
static void software_interrupt_handler(void)
{
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 0;
    schedule();
}

reg_t trap_handler(reg_t mepc, reg_t cause)
{
    reg_t return_pc = mepc;
    reg_t cause_code = cause & 0xfff;

    if (cause & 0x80000000) {
        /* Asynchronous trap - interrupt */
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n");
            software_interrupt_handler();
            break;
        case 7:
            // uart_puts("timer_interruption!\n");
            timer_interrupt_handler();
            break;
        case 11:
            // uart_puts("external interruption!\n");
            external_interrupt_handler();
            break;
        default:
            uart_puts("unknown async exception!\n");
            break;
        }
    } else {
        /* Synchronous trap - exception */
        printf("Sync exceptions! code = %d\n", cause_code);
        panic("OOPS! What can I do!");

        /* try this to skip the instruction that raised the exception */
        // return_pc += 4;  
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