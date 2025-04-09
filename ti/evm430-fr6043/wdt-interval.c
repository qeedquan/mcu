// Test WDT interval mode

#include <msp430.h>

void
init(void)
{
	// Interval mode using ACLK (32 khz) with a 2^15 divider
	// so the interrupt gets generated once per second
	WDTCTL = WDTPW + WDTSSEL_1 + WDTIS_4 + WDTTMSEL_1;

	// Enable interrupt generation for wdt
	SFRIE1 |= WDTIE;

	// LEDs
	PM5CTL0 &= ~LOCKLPM5;
	PJDIR |= BIT3;
	PJOUT &= ~BIT3;

	// Enable interrupts
	_BIS_SR(GIE);
}

int
main(void)
{
	init();
	for (;;)
		;

	return 0;
}

#pragma vector = WDT_VECTOR
__interrupt void
wdt_isr(void)
{
	PJOUT ^= BIT3;
}
