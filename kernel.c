#include "os.h"

/* 
 * Here are some interfaces only called by the
 * kernel, and would not exposed to user ect. 
 */
extern void uart_init(void);
extern void page_init(void);
extern void page_test(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");	
	printf("----------- page_init() ------------\n");
	page_init();
	printf("----------- page_test() ------------\n");
	page_test();
	
	printf("----------- plic_init() ------------\n");
	plic_init();

	printf("----------- timer_init() ------------\n");
	timer_init();

	printf("----------- trap_init() ------------\n");
	trap_init();

	printf("----------- sched_init() ------------\n");
	sched_init();

	printf("------------ os_main() --------------\n");
	os_main();
	
	printf("------------ schedule() -------------\n");
	schedule();

	uart_puts("Would not go here!\n");
	while(1) {}; // stop here!
}
