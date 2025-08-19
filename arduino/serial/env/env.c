// https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html
#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>
#include <util/delay.h>

int
uart_putchar(char c, FILE *fp)
{
	if (c == '\n')
		uart_putchar('\r', fp);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

int
uart_getchar(FILE *fp)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

void
uart_init(void)
{
	// including setbaud.h with BAUD defined setups this value for us
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	// enable 8-bit data
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
	
	// enable tx and rx
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); 
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

#define P(x) printf("%s: %u\n", #x, sizeof(x));

int
main(void)
{
	uart_init();
	stdout = &uart_output;
	stdin = &uart_input;

	// while printing we can cat /dev/ttyACM* to see the output
	while (1) {
		P(char);
		P(short);
		P(int);
		P(long);
		P(long long);
		P(float);
		P(double);
		P(void *);
		printf("\n");
		_delay_ms(1000);
	}

	return 0;
}
