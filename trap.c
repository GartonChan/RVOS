#include "platform.h"
#include "riscv.h"
#include "plic.h"
#include "uart.h"
#include "clint.h"

extern void trap_vector(void);

void trap_init()
{
    /* Set the trap-vector base-address for machine mode */
    w_mtvec((reg_t)trap_vector);
}

static void external_interrupt_handler(void)
{
    int irq = plic_claim();
    
    if (irq == UART0_IRQ) {
        uart_irq();
    } else if (irq) {
        printf("Unexpected interrupt irq = %d\n", irq);
    }

    if (irq) {
        plic_complete(irq);
    }
}

static void timer_interrupt_handler(void)
{
    // ......
    printf("Handle the timer interrupt, INTERVAL = %d\n", TIMER_INTERVAL);
    timer_load(TIMER_INTERVAL);
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