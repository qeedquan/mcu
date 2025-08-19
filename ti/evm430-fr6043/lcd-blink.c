// Display some number and blink

#include "msp430.h"

#define A1_DIGIT LCDM1
#define A1_AlPHA LCDM2
#define A2_DIGIT LCDM3
#define A2_AlPHA LCDM4
#define A3_DIGIT LCDM5
#define A3_AlPHA LCDM6
#define A4_DIGIT LCDM7
#define A4_AlPHA LCDM8

const unsigned char lcd_num[10] = {
    0xFC, // 0
    0x60, // 1
    0xDB, // 2
    0xF3, // 3
    0x67, // 4
    0xB7, // 5
    0xBF, // 6
    0xE4, // 7
    0xFF, // 8
    0xF7, // 9
};

int
main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop WDT

	PJSEL0 = BIT4 | BIT5; // For LFXT

	// Initialize COMS pins and External bias resistor pins
	P6SEL0 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	P6SEL1 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

	// Initialize LCD segments 0 - 32
	LCDCPCTL0 = 0xFFFF;
	LCDCPCTL1 = 0xFFFF;

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// Configure LFXT 32kHz crystal
	// Unlock CS registers
	CSCTL0_H = CSKEY >> 8;
	// Enable LFXT
	CSCTL4 &= ~LFXTOFF;

	// Wait for oscillator fault flag to clear
	do {
		// Clear LFXT fault flag
		CSCTL5 &= ~LFXTOFFG;
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);

	// Lock CS registers
	CSCTL0_H = 0;

	// Initialize LCD_C
	// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
	LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | (LCDMX1 + LCDMX0 + LCDSON);

	// Blink once a second
	LCDCBLKCTL = LCDBLKPRE_5 + LCDBLKMOD_2;

	// Charge pump disabled
	// Internal VLCD, external Rx3 pins, external contrast
	LCDCVCTL = LCDREXT | R03EXT;

	// To use internal bias resistors: LCDCVCTL = 0;
	// Clock synchronization enabled
	LCDCCPCTL = LCDCPCLKSYNC;

	// Clear LCD memory
	LCDCMEMCTL = LCDCLRM;

	// Display number
	A1_DIGIT = lcd_num[1];
	A2_DIGIT = lcd_num[2];
	A3_DIGIT = lcd_num[3];
	A4_DIGIT = lcd_num[4];

	// Turn LCD on
	LCDCCTL0 |= LCDON;

	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();

	return 0;
}
