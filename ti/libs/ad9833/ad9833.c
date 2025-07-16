#include <string.h>
#include "ad9833.h"

#define AD_2POW28 (1UL << 28)

enum {
	AD_B28 = 1 << 13,
	AD_HLB = 1 << 12,
	AD_FSELECT = 1 << 11,
	AD_PSELECT = 1 << 10,
	AD_RESET = 1 << 8,
	AD_SLEEP1 = 1 << 7,
	AD_SLEEP12 = 1 << 6,
	AD_OPBITEN = 1 << 5,
	AD_DIV2 = 1 << 3,
	AD_MODE = 1 << 1
};

#define AD_FREQ1 (1U << 15)
#define AD_FREQ0 (1U << 14)
#define AD_PHASE (1U << 13)

#define SEL_FREQ0 (AD_FREQ0)
#define SEL_FREQ1 (AD_FREQ1)
#define SEL_PHASE0 (AD_FREQ0 | AD_FREQ1 | AD_PHASE)
#define SEL_PHASE1 (AD_FREQ0 | AD_FREQ1 | AD_PHASE)

#define AD_DEFAULT_FREQ 1000
#define AD_DEFAULT_PHASE 0

static void
wr(volatile unsigned char *addr, unsigned char bit, unsigned char val)
{
	if (val)
		*addr |= bit;
	else
		*addr &= ~bit;
}

static void
spi_send(AD9833 *ad, uint16_t data)
{
	int i;

	wr(ad->FSYNC, ad->BIT_FSYNC, 0);
	for (i = 0; i < 16; i++) {
		wr(ad->DATA, ad->BIT_DATA, (data & 0x8000) ? 1 : 0);
		wr(ad->CLK, ad->BIT_CLK, 0);
		wr(ad->CLK, ad->BIT_CLK, 1);
		data <<= 1;
	}
	wr(ad->DATA, ad->BIT_DATA, 0);
	wr(ad->FSYNC, ad->BIT_FSYNC, 1);
}

void
ad9833_init(AD9833 *ad,
            volatile unsigned char *data,
            volatile unsigned char *clk,
            volatile unsigned char *fsync,
            unsigned char bit_data,
            unsigned char bit_clk,
            unsigned char bit_fsync)
{
	memset(ad, 0, sizeof(*ad));
	ad->DATA = data;
	ad->CLK = clk;
	ad->FSYNC = fsync;
	ad->BIT_DATA = bit_data;
	ad->BIT_CLK = bit_clk;
	ad->BIT_FSYNC = bit_fsync;
}

void
ad9833_begin(AD9833 *ad)
{
	wr(ad->CLK, ad->BIT_CLK, 0);
	wr(ad->FSYNC, ad->BIT_FSYNC, 1);

	ad->regctl = 0;
	ad->regctl |= AD_B28;
	spi_send(ad, ad->regctl);

	ad9833_reset(ad, 1);
	ad9833_set_frequency(ad, 0, AD_DEFAULT_FREQ);
	ad9833_set_frequency(ad, 1, AD_DEFAULT_FREQ);
	ad9833_set_phase(ad, 0, AD_DEFAULT_PHASE);
	ad9833_set_phase(ad, 1, AD_DEFAULT_PHASE);
	ad9833_reset(ad, 0);

	ad9833_set_mode(ad, AD9833_MODE_SINE);
	ad9833_set_active_frequency(ad, 0);
	ad9833_set_active_phase(ad, 0);
}

void
ad9833_reset(AD9833 *ad, int hold)
{
	ad->regctl |= AD_RESET;
	spi_send(ad, ad->regctl);
	if (!hold) {
		ad->regctl &= ~AD_RESET;
		spi_send(ad, ad->regctl);
	}
}

int
ad9833_set_mode(AD9833 *ad, int mode)
{
	switch (mode) {
	case AD9833_MODE_OFF:
		ad->regctl &= ~(AD_OPBITEN | AD_MODE);
		ad->regctl |= (AD_SLEEP1 | AD_SLEEP12);
		break;
	case AD9833_MODE_SINE:
		ad->regctl &= ~(AD_OPBITEN | AD_MODE | AD_SLEEP1 | AD_SLEEP12);
		break;
	case AD9833_MODE_SQUARE1:
		ad->regctl &= ~(AD_MODE | AD_SLEEP1 | AD_SLEEP12);
		ad->regctl |= (AD_OPBITEN | AD_DIV2);
		break;
	case AD9833_MODE_SQUARE2:
		ad->regctl &= ~(AD_MODE | AD_DIV2 | AD_SLEEP1 | AD_SLEEP12);
		ad->regctl |= AD_OPBITEN;
		break;
	case AD9833_MODE_TRIANGLE:
		ad->regctl &= ~(AD_OPBITEN | AD_SLEEP1 | AD_SLEEP12);
		ad->regctl |= AD_MODE;
		break;
	default:
		return 0;
	}
	ad->mode = mode;
	spi_send(ad, ad->regctl);

	return 1;
}

static uint32_t
calc_freq(double f)
{
	return (uint32_t)((f * AD_2POW28 / AD_MCLK) + 0.5);
}

static uint16_t
calc_phase(double a)
{
	return (uint16_t)((512.0 * (a / 10) / 45) + 0.5);
}

int
ad9833_set_frequency(AD9833 *ad, int chan, double freq)
{
	uint16_t freqsel;

	switch (chan) {
	case 0:
		freqsel = SEL_FREQ0;
		break;
	case 1:
		freqsel = SEL_FREQ1;
		break;
	default:
		return 0;
	}
	ad->freq = freq;
	ad->regfreq[chan] = calc_freq(freq);
	spi_send(ad, ad->regctl);
	spi_send(ad, freqsel | (uint16_t)(ad->regfreq[chan] & 0x3fff));
	spi_send(ad, freqsel | (uint16_t)((ad->regfreq[chan] >> 14) & 0x3fff));
	return 1;
}

int
ad9833_set_phase(AD9833 *ad, int chan, uint16_t phase)
{
	uint16_t phasesel;

	switch (chan) {
	case 0:
		phasesel = SEL_PHASE0;
		break;
	case 1:
		phasesel = SEL_PHASE1;
		break;
	default:
		return 0;
	}
	ad->phase = phase;
	ad->regphase[chan] = calc_phase(phase);
	spi_send(ad, phasesel | (ad->regphase[chan] & 0xfff));
	return 1;
}

int
ad9833_set_active_frequency(AD9833 *ad, int chan)
{
	switch (chan) {
	case 0:
		ad->regctl &= ~AD_FSELECT;
		break;
	case 1:
		ad->regctl |= AD_FSELECT;
		break;
	default:
		return 0;
	}
	ad->chan = chan;
	spi_send(ad, ad->regctl);
	return 1;
}

int
ad9833_set_active_phase(AD9833 *ad, int chan)
{
	switch (chan) {
	case 0:
		ad->regctl &= ~AD_PSELECT;
		break;
	case 1:
		ad->regctl |= AD_PSELECT;
		break;
	default:
		return 0;
	}
	ad->chan = chan;
	spi_send(ad, ad->regctl);
	return 1;
}
