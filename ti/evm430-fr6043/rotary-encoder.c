/*

Rotary demo using the GPIO pins

Use J8 header to connect the pins

VCC - PJ.1
SW  - PJ.2
DT  - PJ.3
CLK - P4.0

*/

#include <msp430.h>
#include <stdio.h>
#include <stdarg.h>

#define VCCOUT PJOUT
#define VCCDIR PJDIR
#define VCCPIN BIT1

#define SWIN PJIN
#define SWDIR PJDIR
#define SWREN PJREN
#define SWPIN BIT2

#define DTIN PJIN
#define DTDIR PJDIR
#define DTPIN BIT3

#define CLKIN P4IN
#define CLKDIR P4DIR
#define CLKPIN BIT0

void
outc(char ch)
{
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = ch;
}

void
print(const char *fmt, ...)
{
	char buf[128], *ptr;
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	for (ptr = buf; *ptr; ptr++)
		outc(*ptr);
}

void
uartinit(void)
{
	//  Configure CS module
	// MCLK  = 16 MHz from DCOCLK
	// SMCLK = 8MHz from DCOCLK
	// ACLK  = LFXTCLK expected to have a 32.768 KHz
	// Unlock CS registers
	CSCTL0_H = CSKEY >> 8;
	// Set DCO to 8MHz
	CSCTL1 = DCORSEL | DCOFSEL_3;
	// Configure clock dividers all dividers
	CSCTL3 = (DIVA__1 | DIVS__1 | DIVM__1);
	// Set SMCLK = MCLK = DCO, ACLK = LFXTCLK
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL4 |= (LFXTDRIVE_3);
	CSCTL4 &= ~(LFXTOFF);
	CSCTL0_H = 0;

	// GPIO Configuration
	PAOUT = 0;
	PADIR = 0xFFFF;

	PBOUT = 0;
	PBDIR = 0xFFFF;

	PCOUT = 0;
	PCDIR = 0xFFFF;

	PDOUT = 0;
	PDDIR = 0xFFFF;

	PEOUT = 0;
	PEDIR = 0xFFFF;

	// GPIO Configuration for UART mode
	P1SEL0 |= (BIT2 | BIT3);
	P1SEL1 &= ~(BIT2 | BIT3);

	// Configure USCI_A0 for UART mode, 8-bit data, 1 stop bit
	// Put eUSCI in reset
	UCA1CTLW0 = UCSWRST;
	// CLK = SMCLK
	UCA1CTLW0 |= UCSSEL__SMCLK;

	// For BRCLK = SMCLK = 8MHz, and Baud rate = 115200 (See UG)
	UCA1BRW = 4;
	// UCBRSx (bits 7-4) = 0x55, UCBRFx (bits 3-1) = 5, UCOS16 (bit 0) = 1
	UCA1MCTLW = 0x5551;

	// release from reset
	UCA1CTLW0 &= ~UCSWRST;

	// Configure LFXT GPIO pins and start
	PJSEL0 |= BIT4 | BIT5;
}

void
gpioinit(void)
{
	// Set GPIO output to provide voltage to VCC pin
	// This is so we don't have to use a VCC pin that is not on the same header
	VCCDIR |= VCCPIN;
	VCCOUT |= VCCPIN;

	// Set button to be input with internal pullup
	SWDIR &= ~SWPIN;
	SWREN |= SWPIN;

	// Set CLK/DT to be inputs
	CLKDIR &= ~CLKPIN;
	DTDIR &= ~DTPIN;
}

int
main(void)
{
	int button;
	int counter;
	int current_clk, last_clk;
	int current_dt;

	WDTCTL = WDTPW + WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;
	uartinit();
	gpioinit();

	counter = 0;
	current_dt = 0;
	last_clk = !!(CLKIN & CLKPIN);
	for (;;) {
		current_clk = !!(CLKIN & CLKPIN);
		if (current_clk != last_clk && current_clk) {
			current_dt = !!(DTIN & DTPIN);
			if (current_dt != current_clk) {
				counter--;
			} else {
				counter++;
			}
		}
		last_clk = current_clk;

		// Button is low on press
		button = !(SWIN & SWPIN);
		print("button: %d counter: %d\r\n", button, counter);
	}

	return 0;
}
