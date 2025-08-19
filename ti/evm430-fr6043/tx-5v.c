/*

To enable 5V output on the VCC-Tx pin, need to enable the jumper JP1 and JP2 and then enable TxPwr and Tx5VEn pins.

*/

#include <msp430.h>

#define TXPWR_PORT P1OUT
#define TXPWR_PORT_DIR P1DIR
#define TXPWR_PIN BIT0

#define TX5VEN_PORT P1OUT
#define TX5VEN_PORT_DIR P1DIR
#define TX5VEN_PIN BIT1

int
main(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	PM5CTL0 &= ~LOCKLPM5;

	// Initialize TxPwr gpios (clear gpio and configure as output)
	TXPWR_PORT &= ~(TXPWR_PIN);
	TXPWR_PORT_DIR |= (TXPWR_PIN);

	// Initialize Tx5VEn gpios (clear gpio and configure as output)
	TX5VEN_PORT &= ~(TX5VEN_PIN);
	TX5VEN_PORT_DIR |= (TX5VEN_PIN);

	// Turn on the PINs
	TXPWR_PORT |= (TXPWR_PIN);
	TX5VEN_PORT |= (TX5VEN_PIN);

	// Measure the TxPwr pin here, it should output 5v instead of 3.3v
	for (;;) {
		__no_operation();
	}

	return 0;
}
