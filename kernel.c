#include "os.h"

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");	
	printf("----------- page_init() ------------\n");
	page_init();
	printf("----------- page_test() ------------\n");
	page_test();
	
	printf("----------- sched_init() ------------\n");
	sched_init();
	printf("----------- schedule() ------------\n");
	schedule();

	while(1) {}; // stop here!
}
