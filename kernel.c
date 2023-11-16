#include "os.h"

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");	
	page_init();
	printf("------------------------------------\n");
	page_test();
	while(1) {}; // stop here!
}
