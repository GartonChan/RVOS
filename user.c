#include "riscv.h"
#include "os.h"
#include "clint.h"
#include "lock.h"
#include "timer.h"
#include "uart.h"

#define DELAY_TIME 10  // ToDo: accurate delay.


#define USE_LOCK

spin_lock_t lock = { .locked = 0 };

static void user_task0(void)
{
    uart_puts("Task0: Created!\n");

    /* test for compatible task_yield() */
    task_yield();
    uart_puts("Task0: I am back!\n");
    while (1) {
#ifdef USE_LOCK
        acquire_spin_lock(&lock);
        // acquire_spin_lock(); 
        uart_puts("Task0 acquired the spin-lock\n");
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task0: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
        uart_puts("Task0 released the spin-lock\n");
        release_spin_lock(&lock);
        // release_spin_lock();
        task_delay(DELAY_TIME);
#endif
    }
}

static void timer_handler(void *args)
{
    printf("Software timer handler handler!\n");
}

static void user_task1(void)
{
    uart_puts("Task1: Created!\n");

    timer_create(timer_handler, NULL, 5);
    uart_puts("Task1 Timer Created!\n");
    
    while (1) {
#ifdef USE_LOCK
        acquire_spin_lock(&lock);
        // acquire_spin_lock();
        uart_puts("Task1 acquired the spin-lock\n");
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task1: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
        uart_puts("Task1 released the spin-lock\n");
        release_spin_lock(&lock);
        // release_spin_lock();
        task_delay(DELAY_TIME);
#endif
    }
}

static void user_task2(void)
{
    uart_puts("Task2: Created!\n");
    
    timer_create(timer_handler, NULL, 8);
    uart_puts("Task2 Timer Created!\n");

    while (1) {
#ifdef USE_LOCK
        acquire_spin_lock(&lock);
        // acquire_spin_lock();
        uart_puts("Task2 acquired the spin-lock\n");
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task2: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
        uart_puts("Task2 released the spin-lock\n");
        release_spin_lock(&lock);
        // release_spin_lock();
        task_delay(DELAY_TIME);
#endif
        // trap_test();
        // task_yield();
    }
}

void os_main(void)
{
    task_create(user_task0);
    task_create(user_task1);
    task_create(user_task2);
    // trap_test();  /* comment this to avoid raising exception. */
}