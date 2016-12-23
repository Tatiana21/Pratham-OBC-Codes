/*
 * uart.c
 *
 * Created: 31-03-2012 14:38:46
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"

void init_UART0(void )
{

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  
  ///Double Baud Rate
  UCSR0A |= _BV(U2X0);
  ///Enable Reception
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  ///8-bit Data Byte, 2 Stop bits
  UCSR0C |= _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
  ///Set Baud Rate to 9600
  UBRR0L = 103;
  UBRR0H = 0;
}

void init_UART1(void )
{

  UCSR1A = 0;
  UCSR1B = 0;
  UCSR1C = 0;
  
  ///Double Baud Rate
  UCSR1A |= _BV(U2X1);
  ///Enable Reception
  UCSR1B |= _BV(RXEN1) | _BV(TXEN1);
  ///8-bit Data Byte, 2 Stop bits
  UCSR1C |= _BV(USBS1) | _BV(UCSZ11) | _BV(UCSZ10);
  ///Set Baud Rate to 9600
  UBRR1L = 103;
  UBRR1H = 0;
}

void transmit_UART1(char data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSR1A & (_BV(UDRE1))) );
  //* Start transmission
  UDR1 = data;
}

void transmit_UART0(uint8_t data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSR0A & (_BV(UDRE0))) );
  //* Start transmission
  UDR0 = data;
}

uint8_t receive_UART1(void)
{
  while ((UCSR1A & (1 << RXC1)) == 0);
  return UDR1;
}

uint8_t receive_UART0(void)
{
  while ((UCSR0A & (1 << RXC0)) == 0);
  return UDR0;
}

void transmit_string_UART1(char *buffer)
{
  	uint16_t i = 0;
  	///Loop through the data
  	while(buffer[i] != '\0')
  	{
    	///* Wait for empty transmit buffer 
    	while ( !(UCSR1A & (_BV(UDRE1))) );
    	///* Start transmission
    	UDR1 = buffer[i];
		i++;	
	}
}

void transmit_string_UART0(char *buffer)
{
  	uint16_t i = 0;
  	///Loop through the data
  	while(buffer[i] != '\0')
  	{
    	///* Wait for empty transmit buffer 
    	while ( !(UCSR0A & (_BV(UDRE0))) );
    	///* Start transmission
    	UDR0 = buffer[i];
		i++;	
	}
}

