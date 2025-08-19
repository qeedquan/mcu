// Test timer interrupts by using it to toggle some LEDs

#include <stdio.h>
#include <msp430.h>

void
init(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;

	// LED setup
	PJDIR |= BIT3;
	P1DIR |= BIT5;

	// Enable timer interrupt
	TA0CCTL0 = CCIE;
	TA1CCTL0 = CCIE;

	// Use ACLK as a clock source (32 khz) with a divider of /8 so 4000 is ~1 second
	// Count up mode, Interrupt enabled
	// The timer will keep repeatedly counting up until it hits the threshold value and reset from 0
	TA0CTL = TASSEL_1 + MC_1 + ID_3;
	TA0CCR0 = 5000;

	TA1CTL = TASSEL_1 + MC_1 + ID_3;
	TA1CCR0 = 10000;

	// Low power mode, use interrupts to do all the work
	_BIS_SR(CPUOFF + GIE);
}

int
main(void)
{
	init();
	for (;;)
		;

	return 0;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void
timer_a0_isr(void)
{
	PJOUT ^= BIT3;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void
timer_a1_isr(void)
{
	P1OUT ^= BIT5;
}
