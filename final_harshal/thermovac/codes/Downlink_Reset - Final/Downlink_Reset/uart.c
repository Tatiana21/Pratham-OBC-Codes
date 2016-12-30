/*
 * uart.c
 *
 * Created: 02-10-2014 02:08:50
 *  Author: Admin
 */ 

#include "common.h"
#include "uart.h"

/*** Commented by Aayush 2-10-14 to use in Transmit_cc   ***/
/*void main(){
	
	init_UART0();
	while(1){
		
		
		transmit_UART0('K');
	}
	
}
*/
void init_UART0(void)
{
  UCSRA = 0;
  UCSRB = 0;
  UCSRC = 0;
    
  ///Double Baud Rate
  UCSRA |= (1<<U2X);
  ///Enable Reception
  UCSRB |=(1<<RXEN)|(1<<TXEN)| (1<<RXCIE);
  ///8-bit Data Byte, 2 Stop bits
  UCSRC |=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
  ///Set Baud Rate to 9600
  UBRRL = 103;
  UBRRH = 0;
  

  
  
  
}

void transmit_UART0(char data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSRA & (_BV(UDRE))) );
  //* Start transmission
  UDR = data;
}

uint8_t receive_UART0(void)
{
  ///Wait for a Byte to arrive
  while (!(UCSRA & _BV(RXC)));
  return UDR;
}

void send_preflight(char *data, uint16_t size)
{
  uint16_t i = 0;
  
  ///Loop through the data
  while(i < size)
  {
    ///* Wait for empty transmit buffer 
    while ( !(UCSRA & (_BV(UDRE))) );
    ///* Start transmission
    UDR = data[i];
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
    	while ( !(UCSRA & (_BV(UDRE))) );
    	///* Start transmission
    	UDR = buffer[i];
		i++;	
	}
}
