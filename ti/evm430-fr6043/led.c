// Toggle some LEDs

#include <stdio.h>
#include <msp430.h>

enum {
	LED1 = BIT3,
	LED2 = BIT5,
};

void
init(void)
{
	// Disable watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// Unlock power mode 5 which is disabling GPIO port 1 (for LED2)
	// Port J is usable though even with this lock so LED1 will work
	PM5CTL0 &= ~LOCKLPM5;

	// Green LED #1
	PJDIR |= LED1;
	PJOUT |= LED1;

	// Green LED #2
	P1DIR |= LED2;
	P1OUT &= ~LED2;
}

int
main(void)
{
	unsigned count;

	init();

	count = 0;
	for (;;) {
		PJOUT ^= LED1;
		P1OUT ^= LED2;
		printf("%u\n", count++);
	}
	return 0;
}
