// For the EVM-FR6043 eval board, the jumper J3 has to be set for UART TX1/RX1  in order for the output to be routed through the ez-FET (the USB module that connects to the PC), so we can get output from /dev/ttyACM1 on Linux.
// When programming however, the jumper RST/TEST has to be set on connector J3 so to need to swap jumpers when programming and when using the TX1/RX1, or we can get more jumpers somewhere else to use for TX1/RX1

#include <msp430.h>
#include <stdio.h>

// This function is called before main
int
_system_pre_init(void)
{
	// Stop WDT
	WDTCTL = WDTPW + WDTHOLD;

	//  Configure CS module
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

	// GPIO Configuration
	PAOUT = 0;
	PADIR = 0xFFFF;

	PBOUT = 0;
	PBDIR = 0xFFFF;

	PCOUT = 0;
	PCDIR = 0xFFFF;

	PDOUT = 0;
	PDDIR = 0xFFFF;

	PEOUT = 0;
	PEDIR = 0xFFFF;

	// GPIO Configuration for UART mode
	P1SEL0 |= (BIT2 | BIT3);
	P1SEL1 &= ~(BIT2 | BIT3);

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

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// Choose if segment initialization is done or not.
	// Return 1 to run initialization.
	return 1;
}

void
outc(char ch)
{
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = ch;
}

void
outs(const char *str)
{
	for (; *str; str++)
		outc(*str);
}

int
main(void)
{
	char buf[32];
	int cnt;

	cnt = 0;
	for (;;) {
		sprintf(buf, "Hello World: %d!\r\n", cnt);
		outs(buf);
		cnt = (cnt + 1) & 0x3fff;
	}
	return 0;
}
