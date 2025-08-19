/*

This demo tests the hardware comparator.

The comparator +- terminal can be hooked up to a source individually.

In this case:
The - terminal is hooked up to an internal VREF of 2.0v (by setting the register settings)
The + terminal is hooked up to the COMM_IRQ pin, which is channel 2 in the hardware comparator

The + terminal is the one we externally control, we can feed it a voltage input, if it is larger than the - terminal (2v in this case),
the COUT pin will output high, low otherwise.

RxPwr    - COUT
COMM_IRQ - Channel 2 of the comparator
P4.0     - GPIO output that is connected to the COMM_IRQ pin, this allows us to control what the COMM_IRQ is reading in.
           This outputs high when we hold on to a button, low otherwise.

*/

#include <stdio.h>
#include <msp430.h>

void
compinit(void)
{
	// COUT
	P2DIR |= BIT2;
	P2SEL0 |= BIT2;

	// Setup comparator
	// Enable V+, input channel C2
	CECTL0 |= CEIPEN | CEIPSEL_2;

	// Normal power mode
	CECTL1 |= CEPWRMD_1;

	// VREF is applied to -terminal
	CECTL2 |= CERSEL;
	// R-ladder off; bandgap ref voltage (1.2V)
	// supplied ref amplifier to get Vcref=2.0V (CBREFL_2)
	CECTL2 |= CERS_3 | CEREFL_2;

	// Input Buffer Disable C2
	CECTL3 |= BIT2;

	// Turn On ComparatorE
	CECTL1 |= CEON;

	// delay for the reference to settle
	__delay_cycles(75);
}

void
init(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;

	// Set the GPIO as output so it can output to the comparator channel
	P4DIR |= BIT0;

	// Setup button
	PJDIR &= ~BIT0;
	PJREN |= BIT0;

	compinit();
}

int
main(void)
{
	init();

	for (;;) {
		if (!(PJIN & BIT0))
			P4OUT |= BIT0;
		else
			P4OUT &= ~BIT0;
	}
	return 0;
}
