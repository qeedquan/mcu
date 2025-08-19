/*

Implement millis() which counts milliseconds from program starts like Arduino
Do this by using the timer module with a clock of 1mhz to generate an interrupt every millisecond

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <msp430.h>

static volatile uint32_t TIMER_MS_COUNT = 0;

uint32_t
millis(void)
{
	return TIMER_MS_COUNT;
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

void
clockinit(void)
{
	//  Configure CS module
	// MCLK  = 1 MHz from DCOCLK
	// SMCLK = 1 MHz from DCOCLK
	// ACLK  = LFXTCLK expected to have a 32.768 KHz
	// Unlock CS registers
	CSCTL0_H = CSKEY >> 8;
	// Set DCO to 1MHz
	CSCTL1 = DCORSEL | DCOFSEL_0;
	// Configure clock dividers all dividers
	CSCTL3 = (DIVA__1 | DIVS__1 | DIVM__1);
	// Set SMCLK = MCLK = DCO, ACLK = LFXTCLK
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL4 |= (LFXTDRIVE_3);
	CSCTL4 &= ~(LFXTOFF);
	CSCTL0_H = 0;
}

void
uartinit(void)
{
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

	// For BRCLK = SMCLK = 1MHz, and Baud rate = 38400 (See UG)
	UCA1BRW = 26;
	UCA1MCTLW = 0; 

	// release from reset
	UCA1CTLW0 &= ~UCSWRST;

	// Configure LFXT GPIO pins and start
	PJSEL0 |= BIT4 | BIT5;
}

void
timerinit(void)
{
	// Clock source to SMCLK
	TA3CTL |= TASSEL_2;

	// Divider to 1
	TA3CTL |= ID_0;

	// 1000 ticks per second for SMCLK=1mhz
	TA3CCR0 = 1000;

	// Enable interrupt
	TA3CCTL0 |= CCIE;

	// Start timer in UP mode
	TA3CTL |= MC_1;
}

int
main(void)
{
	uint32_t start, now;
	int count;

	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;
	clockinit();
	uartinit();
	timerinit();
	__enable_interrupt();

	count = 0;
	start = millis();
	for (;;) {
		now = millis();
		if ((now - start) >= 1000) {
			print("%d\r\n", count++);
			start = now;
		}
	}

	return 0;
}

#pragma vector = TIMER3_A0_VECTOR
__interrupt void
millis_isr(void)
{
	TIMER_MS_COUNT++;
	TA3CTL &= ~TAIFG;
	TA3CCR0 &= ~CCIFG;
}
