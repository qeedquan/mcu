#include "ad9850.h"

static void
wr(volatile unsigned char *addr, unsigned char bit, unsigned char val)
{
	if (val)
		*addr |= bit;
	else
		*addr &= ~bit;
}

static void
pulse(volatile unsigned char *addr, unsigned char bit)
{
	wr(addr, bit, 1);
	wr(addr, bit, 0);
}

static void
update(AD9850 *ad)
{
	uint32_t f, i;
	uint8_t p;

	f = ad->frequency;
	for (i = 0; i < 32; i++) {
		wr(ad->D7, ad->BIT_D7, f & 1);
		pulse(ad->W_CLK, ad->BIT_W_CLK);
		f >>= 1;
	}

	p = ad->phase;
	for (i = 0; i < 8; i++) {
		wr(ad->D7, ad->BIT_D7, p & 1);
		pulse(ad->W_CLK, ad->BIT_W_CLK);
		p >>= 1;
	}
	pulse(ad->FQ_UD, ad->BIT_FQ_UD);
}

void
ad9850_init(AD9850 *ad,
            volatile unsigned char *w_clk,
            volatile unsigned char *fq_ud,
            volatile unsigned char *d7,
            unsigned char bit_w_clk,
            unsigned char bit_fq_ud,
            unsigned char bit_d7)
{
	ad->W_CLK = w_clk;
	ad->FQ_UD = fq_ud;
	ad->D7 = d7;
	ad->BIT_W_CLK = bit_w_clk;
	ad->BIT_FQ_UD = bit_fq_ud;
	ad->BIT_D7 = bit_d7;

	ad->frequency = 0;
	ad->phase = 0;

	pulse(ad->W_CLK, ad->BIT_W_CLK);
	pulse(ad->FQ_UD, ad->BIT_FQ_UD);
}

void
ad9850_setfreq(AD9850 *ad, double frequency)
{
	ad->frequency = frequency * 4294967296.0 / EX_CLK;
	update(ad);
}

void
ad9850_setphase(AD9850 *ad, uint8_t phase)
{
	ad->phase = phase << 3;
	update(ad);
}

void
ad9850_down(AD9850 *ad)
{
	uint8_t p;
	uint32_t i;

	pulse(ad->FQ_UD, ad->BIT_FQ_UD);
	p = 0x04;
	for (i = 0; i < 8; i++) {
		wr(ad->D7, ad->BIT_D7, p & 1);
		pulse(ad->W_CLK, ad->BIT_W_CLK);
		p >>= 1;
	}
	pulse(ad->FQ_UD, ad->BIT_FQ_UD);
}

void
ad9850_up(AD9850 *ad)
{
	update(ad);
}
