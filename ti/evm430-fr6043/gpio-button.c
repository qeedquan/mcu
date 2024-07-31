/*

Select some GPIO channel, hold down the button to output 1, otherwise 0

*/

#include <stdio.h>
#include <stdarg.h>
#include <msp430.h>

#define nelem(x) (sizeof(x) / sizeof(x[0]))

enum {
	BUTL = BIT0,
	BUTR = BIT1,
	BUTS = BIT2,

	LED1 = BIT3,
	LED2 = BIT5,
};

typedef struct {
	volatile unsigned char *addr;
	int bit;
} GPIO;

static const GPIO gpios[] = {
    // LED
    {&P1OUT, LED2},
    {&PJOUT_L, LED1},

    // I2C
    {&P1OUT, BIT4},
    {&P1OUT, BIT6},
    {&P1OUT, BIT7},
};

int
clamp(int x, int a, int b)
{
	if (x < a)
		x = a;
	else if (x > b)
		x = b;
	return x;
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
	// Green LED #1 and buttons
	PJDIR |= LED1;
	PJOUT &= ~LED1 | (BUTL | BUTR | BUTS);

	// Enable pull up/down resistor for the buttons to make it work
	PJREN |= (BUTL | BUTR | BUTS);

	// Green LED #2
	P1DIR |= LED2 | (BIT4 | BIT6 | BIT7);
	P1OUT &= ~(LED2 | (BIT4 | BIT6 | BIT7));
}

void
init(void)
{
	// Disable watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// Unlock power mode 5 bit
	// Without unlocking it, GPIO port 1 is not usable
	// So if it is locked, LED1 works but not LED2 since LED2 uses port 1
	PM5CTL0 &= ~LOCKLPM5;

	uartinit();
	gpioinit();
}

int
main(void)
{
	int chan, xchan;
	int button, xbutton;

	init();

	button = 0;
	chan = 0;
	for (;;) {
		if (!(PJIN & BUTS))
			*(gpios[chan].addr) |= gpios[chan].bit;
		else
			*(gpios[chan].addr) &= ~gpios[chan].bit;

		xbutton = 0;
		if (!(PJIN & BUTL))
			xbutton |= 1;
		if (!(PJIN & BUTR))
			xbutton |= 2;

		xchan = chan;
		if (!(button & 1) && (xbutton & 1))
			xchan--;
		if (!(button & 2) && (xbutton & 2))
			xchan++;

		button = xbutton;
		if (chan != xchan) {
			chan = clamp(xchan, 0, nelem(gpios) - 1);
			print("GPIO channel: %d\r\n", chan);
		}
	}
	return 0;
}
