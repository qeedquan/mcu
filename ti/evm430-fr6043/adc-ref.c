/*

This test configures one of the PINs to function as an analog channel for ADC measurements.
Sample from the internal bandgap voltage reference (REF0) and output it to UART.
If the PIN is set to be an ADC capture, probing the PIN won't give what it is reading, it will read ~0V

*/

#include <stdio.h>
#include <stdarg.h>
#include <msp430.h>

enum {
	BUTL = BIT0,
	BUTR = BIT1,
	BUTS = BIT2,

	LED1 = BIT3,
	LED2 = BIT5,
};

int level;
int button;

void
outc(char ch)
{
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = ch;
}

int
print(const char *fmt, ...)
{
	va_list ap;
	char b[128];
	int i, n;

	va_start(ap, fmt);
	n = vsnprintf(b, sizeof(b), fmt, ap);
	va_end(ap);
	for (i = 0; i < n; i++)
		outc(b[i]);
	return n;
}

void
init_uart(void)
{
	// Configure CS module
	// MCLK  = 16 MHz from DCOCLK
	// SMCLK = 8MHz from DCOCLK
	// ACLK  = LFXTCLK expected to have a 32.768 KHz
	// Unlock CS registers
	CSCTL0_H = CSKEY >> 8;
	// Set DCO to 8MHz
	CSCTL1 = DCORSEL | DCOFSEL_3;
	// Configure clock dividers all dividers
	CSCTL3 = (DIVA__1 | DIVS__1 | DIVM__1);
	// Set SMCLK = MCLK = DCO, ACLK = LFXTCLK
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL4 |= (LFXTDRIVE_3);
	CSCTL4 &= ~(LFXTOFF);
	CSCTL0_H = 0;

	// Configure USCI_A0 for UART mode, 8-bit data, 1 stop bit
	// Put eUSCI in reset
	UCA1CTLW0 = UCSWRST;
	// CLK = SMCLK
	UCA1CTLW0 |= UCSSEL__SMCLK;

	// For BRCLK = SMCLK = 8MHz, and Baud rate = 115200 (See UG)
	UCA1BRW = 4;
	// UCBRSx (bits 7-4) = 0x55, UCBRFx (bits 3-1) = 5, UCOS16 (bit 0) = 1
	UCA1MCTLW = 0x5551;

	// release from reset
	UCA1CTLW0 &= ~UCSWRST;

	// Configure LFXT GPIO pins and start
	PJSEL0 |= BIT4 | BIT5;

	// GPIO Configuration for UART mode
	P1SEL0 |= (BIT2 | BIT3);
	P1SEL1 &= ~(BIT2 | BIT3);
}

void
init_led_and_buttons(void)
{
	PJDIR |= LED1;
	PJOUT &= ~LED1 | (BUTL | BUTR | BUTS);
	PJREN |= (BUTL | BUTR | BUTS);

	P1DIR |= LED2;
	P1OUT &= ~LED2;
}

void
init_adc(void)
{
	// Configure P1.1 for ADC, this makes it use the ADC functions
	// By default the jumper is on this PIN for this board routing it to COMM_IRQ for I2C
	P1SEL1 |= BIT1;
	P1SEL0 |= BIT1;

	// By default, REFMSTR=1 => REFCTL is used to configure the internal reference
	// If ref generator busy, WAIT
	while (REFCTL0 & REFGENBUSY)
		;

	// Select internal ref = 1.2V
	// Internal Reference ON
	REFCTL0 |= REFVSEL_0 | REFON;

	// Configure ADC12
	// Sample from the internal REF bandgap voltage reference
	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;

	// ADCCLK = MODOSC; sampling timer
	ADC12CTL1 = ADC12SHP;

	// 12-bit conversion results
	ADC12CTL2 |= ADC12RES_2;
	// Enable ADC conversion complete interrupt
	ADC12IER0 |= ADC12IE0;

	// A1 ADC input select; Sample from internal VREF
	ADC12MCTL0 |= ADC12INCH_1 | ADC12VRSEL_1;

	// Wait for reference generator to settle
	while (!(REFCTL0 & REFGENRDY))
		;
}

void
set_revsel(int sel)
{
	int bits[] = {REFVSEL_0, REFVSEL_1, REFVSEL_2, REFVSEL_3};

	while (REFCTL0 & REFGENBUSY)
		;

	REFCTL0 = (REFCTL0 & ~REFVSEL) | bits[sel];
}

void
update_buttons(void)
{
	int xbutton, xlevel;

	xbutton = 0;
	if (!(PJIN & BUTL))
		xbutton |= 1;
	if (!(PJIN & BUTR))
		xbutton |= 2;

	xlevel = level;
	if (!(button & 1) && (xbutton & 1))
		xlevel--;
	if (!(button & 2) && (xbutton & 2))
		xlevel++;

	button = xbutton;
	if (level != xlevel) {
		level = xlevel & 3;
		set_revsel(level);
	}
}

int
main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;

	init_uart();
	init_led_and_buttons();
	init_adc();

	button = 0;
	level = 0;
	set_revsel(level);
	while (1) {
		// Delay between conversions
		__delay_cycles(5000);

		// Sampling and conversion start
		ADC12CTL0 |= ADC12ENC | ADC12SC;

		// LPM0, ADC10_ISR will force exit
		__bis_SR_register(LPM0_bits + GIE);
		// For debug only
		__no_operation();
	}
}

#pragma vector = ADC12_B_VECTOR
__interrupt void
ADC12_ISR(void)
{
	int val;

	switch (__even_in_range(ADC12IV, ADC12IV__ADC12RDYIFG)) {
	case ADC12IV__NONE:
		break; // Vector  0:  No interrupt
	case ADC12IV__ADC12OVIFG:
		break; // Vector  2:  ADC12MEMx Overflow
	case ADC12IV__ADC12TOVIFG:
		break; // Vector  4:  Conversion time overflow
	case ADC12IV__ADC12HIIFG:
		break; // Vector  6:  ADC12BHI
	case ADC12IV__ADC12LOIFG:
		break; // Vector  8:  ADC12BLO
	case ADC12IV__ADC12INIFG:
		break;           // Vector 10:  ADC12BIN
	case ADC12IV__ADC12IFG0: // Vector 12:  ADC12MEM0 Interrupt
		val = ADC12MEM0;
		print("ADC: Level %d Val: %d\r\n", level, val);
		update_buttons();
		__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU

		break; // Clear CPUOFF bit from 0(SR)
	case ADC12IV__ADC12IFG1:
		break; // Vector 14:  ADC12MEM1
	case ADC12IV__ADC12IFG2:
		break; // Vector 16:  ADC12MEM2
	case ADC12IV__ADC12IFG3:
		break; // Vector 18:  ADC12MEM3
	case ADC12IV__ADC12IFG4:
		break; // Vector 20:  ADC12MEM4
	case ADC12IV__ADC12IFG5:
		break; // Vector 22:  ADC12MEM5
	case ADC12IV__ADC12IFG6:
		break; // Vector 24:  ADC12MEM6
	case ADC12IV__ADC12IFG7:
		break; // Vector 26:  ADC12MEM7
	case ADC12IV__ADC12IFG8:
		break; // Vector 28:  ADC12MEM8
	case ADC12IV__ADC12IFG9:
		break; // Vector 30:  ADC12MEM9
	case ADC12IV__ADC12IFG10:
		break; // Vector 32:  ADC12MEM10
	case ADC12IV__ADC12IFG11:
		break; // Vector 34:  ADC12MEM11
	case ADC12IV__ADC12IFG12:
		break; // Vector 36:  ADC12MEM12
	case ADC12IV__ADC12IFG13:
		break; // Vector 38:  ADC12MEM13
	case ADC12IV__ADC12IFG14:
		break; // Vector 40:  ADC12MEM14
	case ADC12IV__ADC12IFG15:
		break; // Vector 42:  ADC12MEM15
	case ADC12IV__ADC12IFG16:
		break; // Vector 44:  ADC12MEM16
	case ADC12IV__ADC12IFG17:
		break; // Vector 46:  ADC12MEM17
	case ADC12IV__ADC12IFG18:
		break; // Vector 48:  ADC12MEM18
	case ADC12IV__ADC12IFG19:
		break; // Vector 50:  ADC12MEM19
	case ADC12IV__ADC12IFG20:
		break; // Vector 52:  ADC12MEM20
	case ADC12IV__ADC12IFG21:
		break; // Vector 54:  ADC12MEM21
	case ADC12IV__ADC12IFG22:
		break; // Vector 56:  ADC12MEM22
	case ADC12IV__ADC12IFG23:
		break; // Vector 58:  ADC12MEM23
	case ADC12IV__ADC12IFG24:
		break; // Vector 60:  ADC12MEM24
	case ADC12IV__ADC12IFG25:
		break; // Vector 62:  ADC12MEM25
	case ADC12IV__ADC12IFG26:
		break; // Vector 64:  ADC12MEM26
	case ADC12IV__ADC12IFG27:
		break; // Vector 66:  ADC12MEM27
	case ADC12IV__ADC12IFG28:
		break; // Vector 68:  ADC12MEM28
	case ADC12IV__ADC12IFG29:
		break; // Vector 70:  ADC12MEM29
	case ADC12IV__ADC12IFG30:
		break; // Vector 72:  ADC12MEM30
	case ADC12IV__ADC12IFG31:
		break; // Vector 74:  ADC12MEM31
	case ADC12IV__ADC12RDYIFG:
		break; // Vector 76:  ADC12RDY
	default:
		break;
	}
}
