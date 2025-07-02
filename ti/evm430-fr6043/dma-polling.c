// Test DMA transfers using polling

#include <stdio.h>
#include <string.h>
#include <msp430.h>

char from[512];
char to[512];

void
init(void)
{
	unsigned i;

	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;

	PJDIR |= BIT3;
	PJOUT &= ~BIT3;

	for (i = 0; i < sizeof(from); i++)
		from[i] = (2 * i) & 0xff;
	memset(to, 0, sizeof(to));

	// Software trigger DMA
	DMACTL0 = DMA0TSEL_0;

	// Set source, dest, and size for transfer
	DMA0SZ = sizeof(from);
	DMA0SA = (unsigned long)from;
	DMA0DA = (unsigned long)to;

	// Block transfer, Enable DMA, Source/Dest address increment
	// Don't use interrupt vector to trigger done, but poll for it to see if its done
	DMA0CTL = DMADT_1 + DMADSTINCR_3 + DMASRCINCR_3 + DMAEN;

	// Trigger DMA request
	DMA0CTL |= DMAREQ;

	// Enable interrupts
	_BIS_SR(GIE);
}

int
main(void)
{
	init();

	// Wait for DMA transfer to be done
	while (!(DMA0CTL & DMAIFG))
		;
	DMA0CTL &= ~DMAEN;

	// Toggle LED to say we are done
	PJOUT |= BIT3;

	// Can set a breakpoint here to inspect memory
	for (;;)
		;

	return 0;
}

#pragma vector = DMA_VECTOR
__interrupt void
dma_isr(void)
{
}
