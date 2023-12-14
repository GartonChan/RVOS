#include "os.h"
#include "lock.h"

#define DELAY_TIME 10  // ToDo: accurate delay.


#define USE_LOCK
#include "riscv.h"

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
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task0: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
        release_spin_lock(&lock);
        // release_spin_lock();
        task_delay(DELAY_TIME);
#endif
    }
}

static void user_task1(void)
{
    uart_puts("Task1: Created!\n");
    while (1) {
#ifdef USE_LOCK
        acquire_spin_lock(&lock);
        // acquire_spin_lock();
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task1: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
        release_spin_lock(&lock);
        // release_spin_lock();
        task_delay(DELAY_TIME);
#endif
    }
}

static void user_task2(void)
{
    uart_puts("Task2: Created!\n");
    while (1) {
#ifdef USE_LOCK
        acquire_spin_lock(&lock);
        // acquire_spin_lock();
#endif
        for (int i = 7; i>0; i--) {
            uart_puts("Task2: Running...\n");
            task_delay(DELAY_TIME/10);
        }
#ifdef USE_LOCK
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