#include "dat.h"
#include "fns.h"

enum {
	GICBASE = 0x8000000,
};

enum {
	// Distributor control register
	GICD_ICDDCR = 0x0 / 4,

	// Interrupt Controller Type Register
	// Shows how many CPU interfaces supported/interrupt lines supported/security extensions/etc
	GICD_TYPER = 0x4 / 4,

	// ID register
	// Shows GIC version/vendor/etc
	GICD_IIDR = 0x8 / 4,

	GICD_IGROUPR = 0x80 / 4,

	// Interrupt Set-Enable Registers
	GICD_ISENABLER0 = 0x100 / 4,

	// Interrupt Clear-Enable Registers
	GICD_ICENABLER0 = 0x180 / 4,

	// Interrupt Set-Pending Registers
	GICD_ISPENDR0 = 0x200 / 4,

	// Interrupt Clear-Pending Registers
	GICD_ICPENDR0 = 0x280 / 4,

	// In older ARM models, these registers are accessed via MSRs instructions
	// CPU Interface control register
	GICC_ICCICR = 0x0 / 4,

	// CPU priority mask
	GICC_PMR = 0x4 / 4,
};

volatile uint32_t *GICD = (void *) GICBASE;
volatile uint32_t *GICC = (void *) (GICBASE + 0x10000);

void
gicdump(void)
{
	printf("GICD\n");
	printf("ICDDCR: %#lx\n", GICD[GICD_ICDDCR]);
	printf("TYPER: %#lx\n", GICD[GICD_TYPER]);
	printf("IIDR: %#lx\n", GICD[GICD_IIDR]);
	printf("ISENABLER0: %#lx\n", GICD[GICD_ISENABLER0]);
	printf("ICENABLER0: %#lx\n", GICD[GICD_ICENABLER0]);
	printf("ISPENDR0: %#lx\n", GICD[GICD_ISPENDR0]);
	printf("ICPENDR0: %#lx\n", GICD[GICD_ICPENDR0]);
	printf("\n");
	printf("GICC\n");
	printf("ICCICR: %#lx\n", GICC[GICC_ICCICR]);
	printf("PMR: %#lx\n", GICC[GICC_PMR]);
	printf("\n");
}

/*

In a uniprocessor system, the steps to configure the GIC:

1. Disable GIC processing of interrupts so we can configure it
2. Configure the GIC to enable signalling to the CPU
3. Set the GIC priority mask to allow interrupts through
4. Enable GIC processing of interrupts

*/

void
gicinit(void)
{
	// disable GIC distributor
	GICD[GICD_ICDDCR] = 0x0;

	// enable signalling of interrupts
	GICC[GICC_ICCICR] = 0x3;

	// pending interrupts with priorities lower than the value in this register will not be taken
	// setting mask to be all 1s allow all interrupts to be taken
	GICC[GICC_PMR] = 0xff;

	// enable GIC distributor
	GICD[GICD_ICDDCR] = 0x1;
}