/*
 * gps_test_2feb.c
 *receive an array from matlab from one pc and receive it on another
 * Created: 2/2/2017 9:51:11 PM
 * Author : tanya
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

 void init_UART0(void)
 {

	 UCSR0A = 0;
	 UCSR0B = 0;
	 UCSR0C = 0;
	 
	 ///Double Baud Rate
	 UCSR0A |= _BV(U2X0);
	 ///Enable Reception
	 UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	 ///8-bit Data Byte, 2 Stop bits
	 UCSR0C |= _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
	 ///Set Baud Rate to 9600
	 UBRR0L = 103;
	 UBRR0H = 0;
	 
 }

void init_UART_MM(void )
{

	UCSR1A = 0;
	UCSR1B = 0;
	UCSR1C = 0;
	
	///Double Baud Rate
	UCSR1A |= _BV(U2X1);
	///Enable Transmission and Reception
	UCSR1B |= _BV(TXEN1) | _BV(RXEN1) |  _BV(RXCIE1);
	///8-bit Data Byte
	UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);
	///Set Baud Rate to 9600
	UBRR1L = 103;
	UBRR1H = 0;
}

void transmit_UART0(uint8_t data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSR0A & (_BV(UDRE0))) );
  //* Start transmission
  UDR0 = data;
}

uint8_t receive_UART0(void)
{
	while ((UCSR0A & (1 << RXC0)) == 0);
	transmit_UART0('v');
	return UDR0;
}

void transmit_UART1(char data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSR1A & (_BV(UDRE1))) );
  //* Start transmission
  UDR1 = data;
}

uint8_t receive_UART1(void)
{
	while ((UCSR1A & (1 << RXC1)) == 0);
	return UDR1;
}

uint8_t lastbyte;
volatile uint8_t flag=0;
int main(void)
{
	sei();
    init_UART0();
	//init_UART_MM();
	//transmit_UART0('t');
    while (1) 
    {
		//transmit_UART1('t');
		//lastbyte = receive_UART0();
	if (flag==1)
	{
		transmit_UART0(lastbyte);
		flag =0;
	}
    }
}

ISR(USART0_RX_vect)
{
	lastbyte = UDR0;
	flag =1;
	//transmit_UART0(lastbyte);
	
}