// Display the time on the LCD using the RTC

#include "msp430.h"

#define A1_DIGIT LCDM1  // Digit A1
#define A1_ALPHA LCDM2  // Alphanumeric part of A1
#define A2_DIGIT LCDM3  // Digit A2
#define A2_ALPHA LCDM4  // Alphanumeric part of A2
#define A3_DIGIT LCDM5  // Digit A3
#define A3_ALPHA LCDM6  // Alphanumeric part of A3
#define A4_DIGIT LCDM7  // Digit A4
#define A4_ALPHA LCDM8  // Alphanumeric part of A4
#define A5_DIGIT LCDM10 // Digit A5
#define A5_ALPHA LCDM11 // Alphanumeric part of A5
#define A6_DIGIT LCDM12 // Digit A6
#define A6_ALPHA LCDM13 // Alphanumeric part of A6
#define BATT LCD14      // Battery indicator

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

void updateLcd(void);

void
updateLcd(void)
{
	A1_DIGIT = lcd_num[(RTCHOUR & 0xF0) >> 4];
	A2_DIGIT = lcd_num[(RTCHOUR & 0x0F)];
	A3_DIGIT = lcd_num[(RTCMIN & 0xF0) >> 4];
	A4_DIGIT = lcd_num[(RTCMIN & 0x0F)];
	A5_DIGIT = lcd_num[(RTCSEC & 0xF0) >> 4];
	A6_DIGIT = lcd_num[(RTCSEC & 0x0F)];
}

int
main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop WDT

	P1DIR |= BIT0; // RTC second indicator
	P1OUT &= ~BIT0;

	PJSEL0 = BIT4 | BIT5; // For LFXT
	// Initialize COMS pins and External bias resistor pins
	P6SEL0 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	P6SEL1 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

	// Initialize LCD segments 0 - 26;
	LCDCPCTL0 = 0xFFFF; // 0-15
	LCDCPCTL1 = 0x07FF; // 16-26

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// Configure LFXT 32kHz crystal
	CSCTL0_H = CSKEY >> 8; // Unlock CS registers
	CSCTL4 &= ~LFXTOFF;    // Enable LFXT
	do {
		CSCTL5 &= ~LFXTOFFG; // Clear LFXT fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG); // Test oscillator fault flag
	CSCTL0_H = 0;              // Lock CS registers

	// Configure RTC_C
	RTCCTL0_H = RTCKEY_H;            // Unlock RTC
	RTCCTL0_L = RTCTEVIE | RTCRDYIE; // enable RTC read ready interrupt
	                                 // enable RTC time event interrupt

	RTCCTL1 = RTCBCD | RTCHOLD | RTCMODE; // RTC enable, BCD mode, RTC hold

	RTCYEAR = 0x2023; // Year = 0x2023
	RTCMON = 0x11;    // Month = 0x14 = November
	RTCDAY = 0x09;    // Day = 0x09 = 9th
	RTCDOW = 0x04;    // Day of week = 0x04 = Thursday
	RTCHOUR = 0x04;   // Hour = 0x04
	RTCMIN = 0x30;    // Minute = 0x30
	RTCSEC = 0x45;    // Seconds = 0x45

	// Initialize LCD_C
	// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
	LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | (LCDMX1 + LCDMX0 + LCDSON) | LCDLP;

	// Charge pump disabled
	// Internal VLCD, external Rx3 pins, external contrast
	LCDCVCTL = LCDREXT | R03EXT;
	// To use internal bias resistors: LCDCVCTL = 0;

	LCDCCPCTL = LCDCPCLKSYNC; // Clock synchronization enabled

	LCDCMEMCTL = LCDCLRM; // Clear LCD memory

	updateLcd();
	A2_ALPHA = 0x04; // Hours-minutes colon
	A4_ALPHA = 0x04; // Minutes-seconds colon

	// Turn LCD on
	LCDCCTL0 |= LCDON;

	RTCCTL1 &= ~(RTCHOLD); // Start RTC

	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();

	return 0;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = RTC_C_VECTOR
__interrupt void
RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(RTC_VECTOR))) RTC_ISR(void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(RTCIV, RTCIV_RT1PSIFG)) {
	case RTCIV_NONE:
		break; // No interrupts
	case RTCIV_RTCOFIFG:
		break;         // RTCOFIFG
	case RTCIV_RTCRDYIFG:  // RTCRDYIFG
		P1OUT ^= 0x01; // Toggles P1.0 every second
		updateLcd();   // Update the segmented LCD
		break;
	case RTCIV_RTCTEVIFG:     // RTCEVIFG
		__no_operation(); // Interrupts every minute
		break;
	case RTCIV_RTCAIFG:
		break; // RTCAIFG
	case RTCIV_RT0PSIFG:
		break; // RT0PSIFG
	case RTCIV_RT1PSIFG:
		break; // RT1PSIFG
	default:
		break;
	}
}
