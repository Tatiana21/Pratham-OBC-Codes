 /* Created: 23-Jan-15 1:42:42 AM
 *  Authors: Abhishek, Harshal and Mohit
 */ 
// For f=14.7456MHz, put UBRRL=191, 
// For f=8.00MHz, put UBBRL=103
//#include "common.h"
#include "uart_8_32.h"


void init_UART(void )
{

  UCSRA = 0;
  UCSRB = 0;
  UCSRC = 0;
  
  ///Double Baud Rate
  UCSRA |= _BV(U2X);
  ///Enable Reception
  UCSRB |= _BV(RXEN); //| _BV(TXEN);
  ///8-bit Data Byte, 2 Stop bits
  UCSRC |= _BV(UCSZ1) | _BV(UCSZ0);
  ///Set Baud Rate to 9600
  
  UBRRL = 103;
  UBRRH = 0;
}

void transmit_UART(char data)
{
  //* Wait for empty transmit buffer 
  while ( !(UCSRA & (_BV(UDRE))) );
  //* Start transmission
  UDR = data;
}


uint8_t receive_UART(void)
{
  while ((UCSRA & (1 << RXC)) == 0);
  return UDR;
}

void transmit_string_UART(char *buffer)
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

/*
* Kill Code to send: 'killpratham'
* Reset Code to send: 'resetpratham'
* To be used for Atmega 32
* Returns uint8_t value 1 if Kill Code Rxd else returns uint8_t value 0
*/
uint8_t isKillCodeRxd() {
	
	unsigned char mystring[20], x, i = 0;
	//Receive the First Character from UART.
	char startChar = UDR;
	
	//Only go ahead with the validation logic IF the first character is 'k'
	if (startChar == 'k') {
		//Keep Receiving till the character 'm' is encountered
		while (((x = receive_UART()) != 'm')) {
			mystring[i] = x;
			i++;

		}
		//Add null character to terminate the string
		mystring[i] = '\0';

		//Actual comparison Logic. First and last character already checked, hence only 'illpratha' is checked.
		//strstr checks whether |mystring| contains "illpratha", this is for the case when some previous command gets pre/suf-fixed with "killpratham".
		if (strstr(mystring, "lliuraptah") != NULL) {
			return 1;
		}
		else {
			return 0;
		}

		//Flush the |mystring| array to contain all '0's.
		for (int j = 0; j < 20; j++) {
			mystring[j] = '0';
		}
		} else if (startChar == 'r') {
		//Keep Receiving till the character 'm' is encountered
		while (((x = receive_UART()) != 'm')) {
			mystring[i] = x;
			i++;

		}
		//Add null character to terminate the string
		mystring[i] = '\0';

		//Actual comparison Logic. First and last character already checked, hence only 'illpratha' is checked.
		//strstr checks whether |mystring| contains "illpratha", this is for the case when some previous command gets pre/suf-fixed with "killpratham".
		if (strstr(mystring, "esetpratha") != NULL) {
			return 2;
		}
		else {
			return 0;
		}

		//Flush the |mystring| array to contain all '0's.
		for (int j = 0; j < 20; j++) {
			mystring[j] = '0';
		}
	}
	
	return 0;

}

