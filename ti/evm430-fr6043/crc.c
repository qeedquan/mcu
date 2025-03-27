// Calculate CRC checksum using CRC registers

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

uint16_t
crc16(const uint8_t *data, size_t len, bool reversed)
{
	uint16_t val;
	size_t i;

	CRCINIRES = 0xffff;
	CRCRESR = 0xffff;

	for (i = 0; i < len; i += 2) {
		val = data[i];
		if (i + 1 < len)
			val |= data[i + 1] << 8;

		if (reversed)
			CRCDIRB = val;
		else
			CRCDI = val;
	}
	return (reversed) ? CRCINIRES : CRCRESR;
}

int
main(void)
{
	static const uint8_t data[] = "hello\n";
	uint16_t sum1, sum2;

	sum1 = crc16(data, sizeof(data) - 1, true);
	sum2 = crc16(data, sizeof(data) - 1, false);
	printf("CRC16: %x %x\n", sum1, sum2);

	return 0;
}
