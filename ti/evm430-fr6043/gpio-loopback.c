/*

Tests loopback, connect a wire from a GPIO output pin to a GPIO input pin.
Whenever the GPIO output pin emits a high signal, the GPIO input pin will read it and light some LEDs.

*/

#include <stdio.h>
#include <msp430.h>

#define nelem(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
	volatile unsigned char *in;
	volatile unsigned char *out;
	volatile unsigned char *dir;
	volatile unsigned char *ren;
	unsigned int pin;
} GPIO;

GPIO led[] = {
    {&PJIN_L, &PJOUT_L, &PJDIR_L, &PJREN_L, BIT3},
    {&P1IN, &P1OUT, &P1DIR, &P1REN, BIT5},
};

GPIO but[] = {
    {&PJIN_L, &PJOUT_L, &PJDIR_L, &PJREN_L, BIT0},
    {&PJIN_L, &PJOUT_L, &PJDIR_L, &PJREN_L, BIT1},
    {&PJIN_L, &PJOUT_L, &PJDIR_L, &PJREN_L, BIT2},
};

GPIO lb[] = {
    {&P1IN, &P1OUT, &P1DIR, &P1REN, BIT3},
    {&P4IN, &P4OUT, &P4DIR, &P4REN, BIT0},
};

void
gpiodir(GPIO *g, int io)
{
	if (io == 'i')
		*g->dir &= ~g->pin;
	else
		*g->dir |= g->pin;
}

void
gpioren(GPIO *g, int en)
{
	if (en)
		*g->ren |= g->pin;
	else
		*g->ren &= ~g->pin;
}

void
gpioout(GPIO *g, int val)
{
	if (val)
		*g->out |= g->pin;
	else
		*g->out &= ~g->pin;
}

int
gpioin(GPIO *g)
{
	return !!(*g->in & g->pin);
}

void
init(void)
{
	unsigned i;

	// Disable watchdog
	WDTCTL = WDTPW | WDTHOLD;

	// Unlock power mode 5 bit
	// Without unlocking it, GPIO port 1 is not usable
	// So if it is locked, LED1 works but not LED2 since LED2 uses port 1
	PM5CTL0 &= ~LOCKLPM5;

	// Set LEDs to output
	for (i = 0; i < nelem(led); i++) {
		gpiodir(&led[i], 'o');
		gpioout(&led[i], 0);
	}

	// Set buttons to be input
	for (i = 0; i < nelem(but); i++) {
		gpiodir(&but[i], 'i');
		gpioren(&but[i], 1);
		gpioout(&but[i], 1);
	}

	// Set loopback, one output and one input
	gpiodir(&lb[0], 'o');
	gpioout(&lb[0], 0);
	gpiodir(&lb[1], 'i');
}

int
main(void)
{
	int val;

	init();
	for (;;) {
		val = !gpioin(&but[0]);
		gpioout(&led[0], val);
		gpioout(&lb[0], val);
		gpioout(&led[1], gpioin(&lb[1]));
	}
	return 0;
}
