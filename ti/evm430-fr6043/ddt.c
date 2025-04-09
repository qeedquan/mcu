// Device block contains all the manufacturing information

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

#define DDT ((void *)0x1a00)
#define SEED ((void *)0x1a30)

typedef struct {
	uint8_t infolen;
	uint8_t crclen;
	uint16_t crcval;
	uint16_t devid;
	uint8_t hwrev;
	uint8_t fwrev;

	uint8_t diertag;
	uint8_t dierlen;
	uint32_t waferid;
	uint16_t diex;
	uint16_t diey;
	uint16_t dietest;
} TLV;

void
init(void)
{
	WDTCTL = WDTPW + WDTHOLD;
}

void
hexdump(uint8_t *buf, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		printf("%x ", buf[i]);
	printf("\n");
}

// TLV is a structure that has all the device block information
void
test_tlv(void)
{
	TLV *t;

	t = DDT;
	printf("Info Length: %d\n", t->infolen);
	printf("Device ID:   %x\n", t->devid);
	printf("CRC:         %d %x\n", t->crclen, t->crcval);
	printf("Revision:    %d %d\n", t->hwrev, t->fwrev);
	printf("\n");

	printf("Wafer ID: %lx\n", t->waferid);
	printf("\n");
}

// The random seed is static and does not change, but it should be different per board
void
test_seed(void)
{
	uint8_t seed[16];
	int i;

	for (i = 0; i < 4; i++) {
		memcpy(seed, SEED, sizeof(seed));
		hexdump(seed, sizeof(seed));
	}
}

int
main(void)
{
	init();

	test_tlv();
	test_seed();

	return 0;
}
