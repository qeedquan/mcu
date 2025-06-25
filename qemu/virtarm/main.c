#include "dat.h"
#include "fns.h"

// this should trigger an infinite reset since we map
// the vectors to the entry point
void
test_svc(void)
{
	asm volatile("svc #0");
}

void
dumpregs(void)
{
	printf("vbar: %#lx\n", rdvbar());
	printf("rdcntfreq: %ld\n", rdcntfreq());
	printf("rdcpsr: %#lx\n", rdcpsr());
	printf("\n");

	gicdump();
	gpiodump();
}

int
main(void)
{
	// setup exception vector handler address
	wrvbar((void*) &_vectors);

	// enable interrupt
	wrcpsr(rdcpsr() & ~0x80);
	gicinit();
	gpioinit();

	dumpregs();

	for (;;)
		;
	return 0;
}
