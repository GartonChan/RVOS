#include "os.h"

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");	
	printf("----------- page_init() ------------\n");
	page_init();
	printf("----------- page_test() ------------\n");
	page_test();
	
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
