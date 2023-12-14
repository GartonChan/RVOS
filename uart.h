#ifndef __UART_H__
#define __UART_H__

#include "platform.h"
/*
 * The UART control registers are memory-mapped at address UART0. 
 * This macro returns the address of one of the control registers.
 */
#define UART0_REG(reg) ((volatile uint8_t *)(UART0_BASE + reg))

/*
 * # Reference:
 * [1]: TECHNICAL DATA ON 16550, http://byterunner.com/16550.html
 *
 * # POWER UP DEFAULTS:
 *   IER = 0: TX/RX holding register interrupts are both disabled
 *   ISR = 1: no interrupt penting
 *   LCR = 0
 *   MCR = 0
 *   LSR = 60 HEX
 *   MSR = BITS 0-3 = 0, BITS 4-7 = inputs
 *   FCR = 0
 *   TX = High
 *   OP1 = High
 *   OP2 = High
 *   RTS = High
 *   DTR = High
 *   RXRDY = High
 *   TXRDY = Low
 *   INT = Low
 */
/* Register Bit Maps */
#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

#define uart_read_reg(reg) (*(UART0_REG(reg)))
#define uart_write_reg(reg, v) (*(UART0_REG(reg)) = (v))

/*
 * LINE STATUS REGISTER (LSR)
 * LSR BIT-0:
 * 0 = no data in receive holding register or FIFO.
 * 1 = data has been receive and saved in the receive holding register or FIFO.
 * ......
 * LSR BIT-5:
 * 0 = transmit holding register is full. 16550 will not accept any data for transmission.
 * 1 = transmitter hold register (or FIFO) is empty. CPU can load the next character.
 * ......
 */
#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

/*
 * INTERRUPT ENABLE REGISTER (IER) 
 * IER BIT-0:
 * 0 = disable the receiver ready interrupt.
 * 1 = enable the receiver ready interrupt.
 * IER BIT-1:
 * 0 = disable the transmitter empty interrupt.
 * 1 = enable the transmitter empty interrupt.
 */
#define IER_RX_READY (1 << 0)
#define IER_TX_EMPTY (1 << 5)

void uart_init(void);
int uart_putc(char ch);
void uart_puts(char *s);
int uart_getc(void);
void uart_irq(void);

int printf(const char* s, ...);
void panic(char *s);

#endif  /* __UART_H__ */
