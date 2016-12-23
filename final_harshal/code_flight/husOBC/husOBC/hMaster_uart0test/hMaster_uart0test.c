 
/****************************************************************************/
/* 					PRATHAM - IITB's Student Satellite                      */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Atmel AVRmega128L                                               */
/* Written for the AVRStudio5 compiler                                      */
/*                                                                          */
/* Author:  Hussain Manasawala, MTech in Electronic Systems, IITB           */
/*                                                                          */
/* Contact: husainmanasa@ee.iitb.ac.in                                      */
/*                                                                          */
/****************************************************************************/
/*
 * hMaster_uart0test.c
 *
 * Created: 31-03-2012 17:18:36
 *  Author: Hussain
 */

#include "common.h"
#include "uart.h"

char rxbuff[20];

/** @brief Interrupt on receiving a byte through UART GPS
 */
#define INTERRUPT_DRIVEN

#ifdef INTERRUPT_DRIVEN
static unsigned char usart_buffer[16];
static unsigned char usart_buffer_head = 0;
static unsigned char usart_buffer_tail = 0;

SIGNAL(SIG_USART0_RECV)
{
	usart_buffer[usart_buffer_head]=UDR1;
	usart_buffer_head++;
	usart_buffer_head &= 0xf;
}
#endif

int read_UART0(unsigned char *buf, int max)
{
#ifdef INTERRUPT_DRIVEN
	int i=0;
	cli();
	while (usart_buffer_head != usart_buffer_tail && i<max) 
	{
		buf[i] = usart_buffer[usart_buffer_tail];
		usart_buffer_tail++;
		usart_buffer_tail &= 0xf;
		i++;
	}
	sei();
	return i;
#else
	while (!(UCSR1A & (1<<RXC1)));
	*buf = UDR1;
	return 1;
#endif
}

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
	char temp;
	
	DDRA = 0xF0;
	PORTA = 0xF0;
	_delay_ms(1000);
	PORTA = 0x00;
	_delay_ms(1000);
	PORTA = 0xF0;
	_delay_ms(1000);
	PORTA = 0x00;
	_delay_ms(1000);
	
	init_UART0();
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('e');
	transmit_UART0('l');
	transmit_UART0('l');
	transmit_UART0('o');
	
	sprintf(array,"\t..This is IITB's Student Satellite...\r");
	transmit_string_UART0(array);
	
	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array2,"\rU r communicating through UART_");
		
/************************************************************/

	while(1){
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	transmit_string_UART0(array2);
	transmit_UART0('0');
	//temp = receive_UART0();
	transmit_UART0('\t');
	//transmit_UART0(temp);
	read_UART0(rxbuff, 10);
	transmit_string_UART0(rxbuff);
	
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
