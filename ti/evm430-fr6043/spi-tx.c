/*

This demo tests SPI mode using the USCI module
Sets it to be a 3 wire (CLK, MOSI, MISO) and continuously send TX data that is an incrementing 8 bit counter

Can view the traffic with a logic analyzer by hooking the probes to the the following:

TxPwr PIN on J10 -> CLK
COMM_TXD on J1 -> MOSI
COMM_RXD on J1 -> MISO

The PINs configured above uses the standard serial port pins so serial on those lines can't be used when using this setup

*/

#include <msp430.h>

volatile unsigned char RXData = 0;
volatile unsigned char TXData;

int
main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

	// Configure GPIO
	P1SEL1 &= ~BIT0 | ~BIT2 | ~BIT3; // USCI_A1 SCLK, MOSI, MISO pins
	P1SEL0 |= BIT0 | BIT2 | BIT3;

	PJSEL0 |= BIT4 | BIT5; // For XT1

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// XT1 Setup
	// Unlock CS registers
	// Set DCO to 1MHz
	// set all dividers
	CSCTL0_H = CSKEY_H;
	CSCTL1 = DCOFSEL_0;
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
	CSCTL4 &= ~LFXTOFF;

	// Test oscillator fault flag
	do {
		// Clear XT1 fault flag
		CSCTL5 &= ~LFXTOFFG;
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);

	// Lock CS registers
	CSCTL0_H = 0;

	// Configure USCI_A0 for SPI operation
	// Put state machine in reset
	UCA1CTLW0 = UCSWRST;
	// 3-pin, 8-bit SPI master
	UCA1CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB;
	// Clock polarity high, MSB
	// ACLK
	UCA1CTLW0 |= UCSSEL__ACLK;
	// Divide by 2
	UCA1BRW = 0x02;
	// No modulation
	UCA1MCTLW = 0;
	// Initialize USCI state machine
	UCA1CTLW0 &= ~UCSWRST;
	// Enable USCI_A0 RX interrupt
	UCA1IE |= UCRXIE;

	// Holds TX data
	TXData = 0x1;

	while (1) {
		UCA1IE |= UCTXIE;
		__bis_SR_register(LPM0_bits | GIE); // CPU off, enable interrupts
		__delay_cycles(2000);               // Delay before next transmission
		TXData++;                           // Increment transmit data
	}
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void
USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA1IV, USCI_SPI_UCTXIFG)) {
	case USCI_NONE:
		break;
	case USCI_SPI_UCRXIFG:
		RXData = UCA1RXBUF;
		UCA1IFG &= ~UCRXIFG;
		__bic_SR_register_on_exit(LPM0_bits); // Wake up to setup next TX
		break;
	case USCI_SPI_UCTXIFG:
		UCA1TXBUF = TXData; // Transmit characters
		UCA1IE &= ~UCTXIE;
		break;
	default:
		break;
	}
}
