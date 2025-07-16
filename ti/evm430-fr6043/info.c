// Dump info about the system

#include <stdio.h>
#include <msp430.h>

void
init(void)
{
	WDTCTL = WDTPW | WDTHOLD;
}

void
dumpregs(void)
{
	unsigned short r4, r5;
	unsigned short sr, sp;

	r4 = __get_R4_register();
	r5 = __get_R5_register();
	sr = __get_SR_register();
	sp = __get_SP_register();

	printf("Regs\n");
	printf("R4: %x R5: %x SR: %x SP: %x\n", r4, r5, sr, sp);
	printf("\n");
}

void
dumpclocks(void)
{
	printf("Clock registers\n");
	printf("CSCTL0: %x\n", CSCTL0);
	printf("CSCTL1: %x\n", CSCTL1);
	printf("CSCTL2: %x\n", CSCTL2);
	printf("CSCTL3: %x\n", CSCTL3);
	printf("CSCTL4: %x\n", CSCTL4);
	printf("CSCTL5: %x\n", CSCTL5);
	printf("CSCTL6: %x\n", CSCTL6);
	printf("\n");
}

void
dumpaddrs(void)
{
	printf("Addresses\n");
	printf("WDT_A_BASE:    %x\n", WDT_A_BASE);
	printf("EUSCI_A0_BASE: %x\n", EUSCI_A0_BASE);
	printf("DIO_BASE:      %x\n", DIO_BASE);
	printf("PMM_BASE:      %x\n", PMM_BASE);
	printf("RAMCTL_BASE:   %x\n", RAMCTL_BASE);
	printf("PA_BASE:       %x\n", PA_BASE);
	printf("PB_BASE:       %x\n", PB_BASE);
	printf("PC_BASE:       %x\n", PC_BASE);
	printf("PD_BASE:       %x\n", PD_BASE);
	printf("PE_BASE:       %x\n", PE_BASE);
	printf("PJ_BASE:       %x\n", PJ_BASE);
	printf("P1_BASE:       %x\n", P1_BASE);
	printf("P2_BASE:       %x\n", P2_BASE);
	printf("P3_BASE:       %x\n", P3_BASE);
	printf("P4_BASE:       %x\n", P4_BASE);
	printf("P5_BASE:       %x\n", P5_BASE);
	printf("P6_BASE:       %x\n", P6_BASE);
	printf("P7_BASE:       %x\n", P7_BASE);
	printf("P8_BASE:       %x\n", P8_BASE);
	printf("P9_BASE:       %x\n", P9_BASE);
	printf("\n");
}

int
main(void)
{
	init();
	dumpregs();
	dumpclocks();
	dumpaddrs();
	return 0;
}
