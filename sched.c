#include "os.h"

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
static int _top = 0;
static int _current = -1;

void sched_init()
{
    w_mscratch(0);
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

static void user_task0(void)
{
    uart_puts("Task0: Created!\n");
    while (1) {
        uart_puts("Task0: Running...\n");
        task_delay(10000);
        task_yield();
    }
}

static void user_task1(void)
{
    uart_puts("Task1: Created!\n");
    while (1) {
        uart_puts("Task1: Running...\n");
        task_delay(10000);
        task_yield();
    }
}

static void user_task2(void)
{
    uart_puts("Task2: Created!\n");
    while (1) {
        uart_puts("Task2: Running...\n");
        task_delay(10000);
        // trap_test();
        task_yield();
    }
}

void os_main(void)
{
    task_create(user_task0);
    task_create(user_task1);
    task_create(user_task2);
    trap_test();  // call here why not exception occur？ -> occured but without uart_puts
}