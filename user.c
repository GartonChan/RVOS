#include "os.h"

#define DELAY_TIME 10  // ToDo: accurate delay.

static void user_task0(void)
{
    uart_puts("Task0: Created!\n");

    /* test for compatible task_yield() */
    task_yield();
    uart_puts("Task0: I am back!\n");
    
    while (1) {
        uart_puts("Task0: Running...\n");
        task_delay(DELAY_TIME);
        // task_yield();
    }
}

static void user_task1(void)
{
    uart_puts("Task1: Created!\n");
    while (1) {
        uart_puts("Task1: Running...\n");
        task_delay(DELAY_TIME);
        // task_yield();
    }
}

static void user_task2(void)
{
    uart_puts("Task2: Created!\n");
    while (1) {
        uart_puts("Task2: Running...\n");
        task_delay(DELAY_TIME);
        // trap_test();
        // task_yield();
    }
}

void os_main(void)
{
    task_create(user_task0);
    task_create(user_task1);
    task_create(user_task2);
    // trap_test();  /* comment this to avoid exception. */
}