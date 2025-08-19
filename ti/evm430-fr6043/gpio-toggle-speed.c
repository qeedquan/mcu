/*

Tests how fast we can toggle GPIOs based on clock setup

*/

#include <stdint.h>
#include <stdio.h>
#include <msp430.h>

#define nelem(x) (sizeof(x) / sizeof(x[0]))

enum {
	BUTL = BIT0,
	BUTR = BIT1,
	BUTS = BIT2,
};

/*

Used a logic analyzer like Salae to measure.
Set the MCLK to be using the DCO clock which is the fastest we can use

*/
void
clocksource(int dcorsel, int dcofsel, int divider)
{
	uint16_t dcofsel_lut[] = {DCOFSEL_0, DCOFSEL_1, DCOFSEL_2, DCOFSEL_3, DCOFSEL_4, DCOFSEL_5, DCOFSEL_6};
	uint16_t divider_lut[] = {DIVM__1, DIVM__2, DIVM__4, DIVM__8, DIVM__16, DIVM__32};
	uint16_t val;

	CSCTL0_H = CSKEY >> 8;

	// Configure the clock module
	val = 0;
	if (dcorsel)
		val |= DCORSEL;
	val |= dcofsel_lut[dcofsel];
	CSCTL1 = val;

	// Set SMCLK = MCLK = DCO, ACLK = LFXTCLK
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;

	// Configure clock dividers all dividers
	CSCTL3 = (DIVA__1 | DIVS__1 | divider_lut[divider - 1]);
	CSCTL0_H = 0;
}

void
gpioinit(void)
{
	// Green LED #1 and buttons
	PJOUT &= (BUTL | BUTR | BUTS);

	// Enable pull up/down resistor for the buttons to make it work
	PJREN |= (BUTL | BUTR | BUTS);

	// Green LED #2
	P1DIR |= (BIT4 | BIT6 | BIT7);
	P1OUT &= ~(BIT4 | BIT6 | BIT7);
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
	gpioinit();
}

int
main(void)
{
	int dcorsel, dcofsel, divider;
	int button, xbutton;

	init();

	button = 0;
	dcorsel = 1;
	dcofsel = 6;
	divider = 2;
	clocksource(dcorsel, dcofsel, divider);
	for (;;) {
		xbutton = 0;
		if (!(PJIN & BUTL))
			xbutton |= 1;
		if (!(PJIN & BUTR))
			xbutton |= 2;
		if (!(PJIN & BUTS))
			xbutton |= 4;

		if (!(button & 1) && (xbutton & 1))
			dcorsel ^= 1;
		if (!(button & 2) && (xbutton & 2))
			dcofsel = (dcofsel + 1) % 6;
		if (!(button & 4) && (xbutton & 4)) {
			divider = (divider + 1) % 7;
			if (divider == 0)
				divider = 1;
		}
		if (button != xbutton) {
			clocksource(dcorsel, dcofsel, divider);
			button = xbutton;
		}

		P1OUT ^= BIT4;
	}
	return 0;
}
