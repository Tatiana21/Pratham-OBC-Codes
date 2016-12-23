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
 * hMaster_uart1test.c
 *
 * Created: 31-03-2012 17:09:48
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"

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
	
	init_UART1();
  
	transmit_UART1('\r');
	transmit_UART1('\r');
	transmit_UART1('H');
	transmit_UART1('e');
	transmit_UART1('l');
	transmit_UART1('l');
	transmit_UART1('o');
	
	sprintf(array,"\t..This is IITB's Student Satellite...\r");
	transmit_string_UART1(array);

	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array2,"\rU r communicating through UART_");
		
/************************************************************/

	while(1){
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	transmit_UART1('\r');
	transmit_UART1('\r');
	transmit_UART1('H');
	transmit_UART1('i');
	
	transmit_string_UART1(array1);
	transmit_string_UART1(array2);
	transmit_UART1('1');
	temp = receive_UART1();
	transmit_UART1('\t');
	transmit_UART1(temp);
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
