// Test some buttons

#include <stdio.h>
#include <msp430.h>

enum {
	BUTL = BIT0,
	BUTR = BIT1,
	BUTS = BIT2,

	LED1 = BIT3,
	LED2 = BIT5,
};

void
init(void)
{
	// Disable watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// Unlock power mode 5 bit
	// Without unlocking it, GPIO port 1 is not usable
	// So if it is locked, LED1 works but not LED2 since LED2 uses port 1
	PM5CTL0 &= ~LOCKLPM5;

	// Green LED #1 and buttons
	PJDIR |= LED1;
	PJOUT &= ~LED1 | (BUTL | BUTR | BUTS);

	// Enable pull up/down resistor for the buttons to make it work
	PJREN |= (BUTL | BUTR | BUTS);

	// Green LED #2
	P1DIR |= LED2;
	P1OUT &= ~LED2;
}

int
main(void)
{
	init();
	for (;;) {
		if (!(PJIN & BUTL))
			PJOUT |= LED1;
		else
			PJOUT &= ~LED1;

		if (!(PJIN & BUTR))
			P1OUT |= LED2;
		else
			P1OUT &= ~LED2;
	}
	return 0;
}
