#include "dat.h"
#include "fns.h"

enum {
	GICBASE = 0x8000000,
};

enum {
	ICDDCR = 0x0 / 4,
};

volatile uint32_t *GICD = (void *) GICBASE;
volatile uint32_t *GICC = (void *) (GICBASE + 0x10000);

void
dumpgic(void)
{
	printf("GIC\n");
	printf("ICDDCR: %#lx\n", GICD[ICDDCR]);
}