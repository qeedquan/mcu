#include "dat.h"
#include "fns.h"

enum {
	DATA = 0x0 / 4,
	DIR = 0x400 / 4,
	IS = 0x404 / 4,
	IBE = 0x408 / 4,
	IEV = 0x40c / 4,
	IE = 0x410 / 4,
	RIS = 0x414 / 4,
	MIS = 0x418 / 4,
	AFSEL = 0x420 / 4,
	ID0 = 0xfe0 / 4,
};

volatile uint32_t *GPIO = (volatile uint32_t *) 0x9030000;

void
gpiodump(void)
{
	int i;

	printf("GPIO:\n");
	printf("DATA: %#lx\n", GPIO[DATA]);
	printf("DIR: %#lx\n", GPIO[DIR]);
	printf("IS: %#lx\n", GPIO[IS]);
	printf("IBE: %#lx\n", GPIO[IBE]);
	printf("IEV: %#lx\n", GPIO[IEV]);
	printf("IE: %#lx\n", GPIO[IE]);
	printf("RIS: %#lx\n", GPIO[RIS]);
	printf("MIS: %#lx\n", GPIO[MIS]);
	printf("AFSEL: %#lx\n", GPIO[AFSEL]);
	printf("ID: ");
	for (i = 0; i < 8; i++) {
		printf("%#lx ", GPIO[ID0 + i]);
	}
	printf("\n");
	printf("\n");
}

void
gpioinit(void)
{
	// set all gpio to be output
	GPIO[DIR] = 0xff;

	// level trigger
	GPIO[IS] = 0xff;

	// unmask irqs
	GPIO[IE] = 0xff;

	GPIO[DATA] = 0xff;
	GPIO[DATA] = 0x00;
}