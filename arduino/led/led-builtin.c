#include <avr/io.h>
#include <util/delay.h>

void
toggle(int val)
{
	PORTB = val;
	_delay_ms(50);
}

int
main(void)
{
	DDRB = 0xff;
	for (;;) {
		toggle(0xff);
		toggle(0x00);
	}
	return 0;
}
