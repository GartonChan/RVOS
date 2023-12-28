#include "riscv.h"
#include "clint.h"
#include "syscall.h"

extern void uart_puts(char *s);
extern void panic(char *s);

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

void sched_init(void)
{
    w_mscratch((reg_t)&os_main_ctx);  // set mscratch = 0(init value) -> &os_main_ctx
    // ctx_task.sp = (reg_t) &task_stack[STACK_SIZE-1];
    // ctx_task.ra = (reg_t) user_task0;
}

void schedule(void)
{
    if (_top <= 0) {
        panic("No tasks to run.\n");
    }
    /* tasks run in turn (as a round) */
    _current = (_current + 1) % _top;  /* schedule tasks in turn */
    struct context *next = &(ctx_tasks[_current]);
    // printf("next schedule task is task%d\n", _current);
    switch_to(next);
}

/* API for creating a new task within the maximum number */
int task_create(void (*start_routin)(void))
{
    if (_top < MAX_TASKS){  /* check the number of existed tasks */
        ctx_tasks[_top].sp = (reg_t)&task_stack[_top][STACK_SIZE-1];
        // ctx_tasks[_top].ra = (reg_t)start_routin;
        ctx_tasks[_top].pc = (reg_t)start_routin;
        _top++;
        return 0;
    } else {
        uart_puts("task_create failed, the number of tasks over maximum.");
        return -1;
    }
    
}

/* 
 * called by the task to hand out cpu actively, 
 * then a new task will take the cpu to run itself. 
 */
void task_yield()
{   
    /* Cause software interrupt to achieve schedule */
    uart_puts("task_yield\n");
    // int id = r_mhartid();
    uint32_t hid;
    printf("ptr of hid = 0x%x\n", &hid);
    gethid(&hid);
    printf("hid = %d in task_yield\n", hid);
    *(uint32_t *)CLINT_MSIP(hid) = 1;  /* equal to set mip.MSIP valid */
    // schedule();  /* can not be called in U-mode */
}

/* delay function called by task to hold */
void task_delay(volatile int count)
{
    count *= CLINT_TIMEBASE_FREQ;
    while (count--);
}
