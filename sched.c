#include "riscv.h"

/* defined in entry.S */
extern void switch_to(struct context *next_task);

#define MAX_TASKS 10
#define STACK_SIZE 1024

/*
 * In the standard RISC-V calling convention, sp is
 * always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];
struct context os_main_ctx;  // allocate for os_main to solve a new exception 
static int _top = 0;
static int _current = -1; 

void sched_init()
{
    w_mscratch((reg_t)&os_main_ctx);  // set mscratch = 0(init value) -> &os_main_ctx
    // ctx_task.sp = (reg_t) &task_stack[STACK_SIZE-1];
    // ctx_task.ra = (reg_t) user_task0;
}

void schedule()
{
    _current = (_current + 1) % _top;  /* schedule tasks in turn */
    struct context *next = &(ctx_tasks[_current]);
    switch_to(next);
}

/* API for creating a new task within the maximum number */
int task_create(void (*start_routin)(void))
{
    ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE-1];
    ctx_tasks[_top].ra = (reg_t) start_routin;
    _top++;
    return 0;
}

/* called by the task to give up cpu actively */
void task_yield()
{
    schedule();
}

/* delay function called by task to hold */
void task_delay(volatile int count)
{
    count *= 50000;
    while (count--);
}
