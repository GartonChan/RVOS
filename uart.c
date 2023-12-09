#include "platform.h"
#include "types.h"
#include "uart.h"

void uart_init(void)
{
    /* write 0x00 to Interrupt Enable Register to disable interrupts */
    uart_write_reg(IER, 0x00);
    
    /*
	 * Setting baud rate. Just a demo here if we care about the divisor,
	 * but for our purpose [QEMU-virt], this doesn't really do anything.
	 *
	 * Notice that the divisor register DLL (divisor latch least)
     * and DLM (divisor latch most) have the same base address
     * as the receiver/transmitter and the interrupt enable register.
     * 
     * To change what the base address points to, we open the "divisor latch"
     * by writing 1 into the Divisor Latch Access Bit(DLAB),
     * which is bit index 7 of the Line Control Register (LCR).
	 *
	 * Regarding the baud rate value, see [1] "BAUD RATE GENERATOR PROGRAMMING TABLE".
     * "Customize Baud Rates can be achieved by selecting proper 
     * divisor values for MSBand LSB of baud rate generator."
	 * According to the reference[1], we use 38.4K when 1.8432 MHZ crystal, so
     * the corresponding value is 3. And due to the divisor register is
     * two bytes (16 bits), so we need to split the value of 3 (0x0003)
     * into two bytes, DLL stores the low byte, DLM stores the high byte.
	 */
    uint8_t lcr = uart_read_reg(LCR);
    uart_write_reg(LCR, lcr | (1 << 7));
    uart_write_reg(DLL, 0x03);
    uart_write_reg(DLM, 0x00);

    /*
	 * The LCR is used to specify the asynchronous data communication format. 
     * - number of the word length: 8 bits -> bit-1 = bit-0 = 1
	 * - number of stop bits：1 bit when word length is 8 bits -> bit-2 = 0
	 * - no parity -> bit-5 = bit-4 = bit-3 = 0
	 * - no break control -> bit-6 = 0
     * - disabled baud latch -> bit-7 = 0
     * so the configuration is 0b0000011 => 0x03
	 */
    lcr = 0;
    uart_write_reg(LCR, lcr | (3 << 0));
    
    /* Enable receive interrupts */
    uint8_t ier = uart_read_reg(IER);
    uart_write_reg(IER, ier | IER_RX_READY);
    // uart_write_reg(IER, ier | IER_TX_EMPTY);
}

int uart_putc(char ch)
{
    /* 
     * check if the LSR bit5 is euqal to zero, which means that THR is empty
     * or it will be busy-waiting here until THR become empty.
     */
    while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
    return uart_write_reg(THR, ch);  // THR is empty, then write a char in it.
}

void uart_puts(char *s)
{
    while(*s) {  /* iterate for each character in the input string */
        uart_putc(*s++);
    }
}

int uart_getc(void)
{
    /* 
     * check if the LSR bit0 is euqal to one, which means that RHR is ready
     * or it will be busy-waiting here until THR become empty.
     */
    if(uart_read_reg(LSR) & LSR_RX_READY) {
        return uart_read_reg(RHR);
    } else {  /* RX_READY==0 means not ready */ 
        return -1;
    }
}

/* 
 * Handle a uart interrupt, raised because input has
 * arrived, called from trap.c
 */
void uart_irq(void)
{
    int ch;
    while (1) {
        ch = uart_getc();
        if (ch == -1) {
            break;
        }
        else {
            uart_putc((char)ch);
            uart_putc('\n');
        }
    }
}