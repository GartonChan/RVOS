#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next_task);

#define STACK_SIZE 1024
/*
 * In the standard RISC-V calling convention, sp is
 * always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[STACK_SIZE];
struct context ctx_task;

static void w_mscratch(reg_t x)
{
    asm volatile("csrw mscratch, %0" : : "r" (x));  // ?   
}

void user_task0(void);
void sched_init()
{
    w_mscratch(0);
    ctx_task.sp = (reg_t) &task_stack[STACK_SIZE];
    ctx_task.ra = (reg_t) user_task0;
}


void schedule()
{
    struct context *next = &ctx_task;
    switch_to(next);
}

void task_delay(volatile int count)
{
    count *= 50000;
    while (count--);
}

void user_task0(void)
{
    uart_puts("Task0: Created!\n");
    while (1) {
        uart_puts("Task0: Running...\n");
        task_delay(10000);
    }
}