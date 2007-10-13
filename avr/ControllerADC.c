#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 12000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int can_test = 1;
int adcval = 0;

ISR(ADC_vect)
{
	can_test = 1;
	adcval = ADC;
}

void USART_Init( unsigned int ubrr)
{

	int i = 0;
// Set baud rate 
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
// Enable receiver and transmitter 
	UCSRB = (1<<RXEN)|(1<<TXEN);
// Set frame format: 8data, 2stop bit 
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

	ADCSRA = _BV(ADEN) | _BV(ADATE);
	sei();
	
	ADCSRA |= _BV(ADIE);
	ADCSRA |= _BV(ADSC);
	

}

int main()
{
	USART_Init ( MYUBRR );
		can_test = 0;
	while(1)
	{
		if(can_test)
		{
			can_test = 0;
			while ( !( UCSRA & (1<<UDRE)) );
			UDR = (adcval/4);
			while ( !( UCSRA & (1<<UDRE)) );
			UDR = 0xA;
		}
	}
	return 0;
}
