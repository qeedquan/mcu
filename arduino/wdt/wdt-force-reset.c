#include <avr/wdt.h>
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
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
	
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); 
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
int count;

int
main(void)
{
	uart_init();
	stdout = &uart_output;
	stdin = &uart_input;

	// force reset every 8 seconds
	wdt_enable(WDTO_8S);

	// when we reset the variables are 0s
	printf("initialization %d\n", count++);
	for (;;)
		_delay_ms(1000);

	return 0;
}
