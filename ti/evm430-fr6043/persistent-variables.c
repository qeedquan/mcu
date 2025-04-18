// Tests persistent variables

#include <msp430.h>
#include <stdio.h>
#include <stdarg.h>

// Persistent variables are variables that are initialized a code load time but not afterwards
// this is different from a regular static variable where it is initialized to zero on every startup
// this is useful for placing variables in non-volatile memory (this pragma moves the variables into .TI.persistent section)
// in which a reset or power cycle will not reset the variable value since it is stored in non-volatile memory (usually fram)
#pragma PERSISTENT(persist)
int persist = 0;

void
init(void)
{
	// Stop WDT
	WDTCTL = WDTPW + WDTHOLD;

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

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;
}

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

int
main(void)
{
	init();

	persist = (persist + 10) % 10000;
	for (;;) {
		print("Persistent %d\r\n", persist);
	}

	return 0;
}
