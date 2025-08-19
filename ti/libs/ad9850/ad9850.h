#ifndef _AD9850_H_
#define _AD9850_H_

#include <stdint.h>

#ifndef EX_CLK
#define EX_CLK 125000000.0
#endif

typedef struct {
	volatile unsigned char *W_CLK;
	volatile unsigned char *FQ_UD;
	volatile unsigned char *D7;

	unsigned char BIT_W_CLK;
	unsigned char BIT_FQ_UD;
	unsigned char BIT_D7;

	uint32_t frequency;
	uint8_t phase;
} AD9850;

void ad9850_init(AD9850 *ad,
                 volatile unsigned char *w_clk,
                 volatile unsigned char *fq_ud,
                 volatile unsigned char *d7,
                 unsigned char bit_w_clk,
                 unsigned char bit_fq_ud,
                 unsigned char bit_d7);

void ad9850_setfreq(AD9850 *ad, double frequency);
void ad9850_setphase(AD9850 *ad, uint8_t phase);
void ad9850_down(AD9850 *ad);
void ad9850_up(AD9850 *ad);

#endif
