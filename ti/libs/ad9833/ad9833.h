#ifndef _AD9833_H_
#define _AD9833_H_

#include <stdint.h>

#ifndef AD_MCLK
#define AD_MCLK 25000000UL
#endif

typedef struct {
	volatile unsigned char *DATA;
	volatile unsigned char *CLK;
	volatile unsigned char *FSYNC;

	unsigned char BIT_DATA;
	unsigned char BIT_CLK;
	unsigned char BIT_FSYNC;

	uint16_t regctl;
	uint32_t regfreq[2];
	uint32_t regphase[2];

	int chan;
	int mode;
	double freq;
	uint16_t phase;
} AD9833;

enum {
	AD9833_MODE_OFF = 0,
	AD9833_MODE_SINE,
	AD9833_MODE_SQUARE1,
	AD9833_MODE_SQUARE2,
	AD9833_MODE_TRIANGLE,
	AD9833_MODE_MAX
};

void ad9833_init(AD9833 *ad,
                 volatile unsigned char *data,
                 volatile unsigned char *clk,
                 volatile unsigned char *fsync,
                 unsigned char bit_data,
                 unsigned char bit_clk,
                 unsigned char bit_fsync);

void ad9833_begin(AD9833 *ad);
void ad9833_reset(AD9833 *ad, int hold);

int ad9833_set_mode(AD9833 *ad, int mode);
int ad9833_set_frequency(AD9833 *ad, int chan, double freq);
int ad9833_set_phase(AD9833 *ad, int chan, uint16_t phase);

int ad9833_set_active_frequency(AD9833 *ad, int chan);
int ad9833_set_active_phase(AD9833 *ad, int chan);

#endif
