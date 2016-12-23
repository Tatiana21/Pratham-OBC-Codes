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
 * hSlave_eepromtest.c
 *
 * Created: 31-03-2012 17:56:31
 *  Author: Hussain
 */

#include "common.h"
#include "eeprom.h"
#include "uart.h"

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
  
	DDRA = 0x0F;
	PORTA = 0x0F;
	_delay_ms(1000);
	PORTA = 0x00;
	_delay_ms(1000);
	PORTA = 0x0F;
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

	sprintf(array1,"\tThis is HUSSAIN's OBC-Slave code...");
	sprintf(array2,"\rData read from EEPROM :\t");
		
/************************************************************/
	char buffer[100];
	uint8_t frame[256], i=0;
	///EEPROM function to initialise I/O
	ioinit();
  
/************************************************************/
	while(1){
	PORTA = 0x0A;
	_delay_ms(500);
	PORTA = 0x05;
	_delay_ms(500);
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	transmit_string_UART0(array2);
	
/************************************************************/
	PORTA = 0x0A;
	_delay_ms(500);
	PORTA = 0x05;
	_delay_ms(500);
	
	write_addr = HALF_ADDR - (HALF_ADDR % EEP_FRAME_SIZE);
	write_addr -= EEP_FRAME_SIZE;
	sprintf(buffer,"Data written %d times",i);
	i++;
	write_frame_to_eeprom((uint8_t *)buffer);
	write_frame_to_eeprom((uint8_t *)buffer);
  
	read_addr = HALF_ADDR;
	read_frame_from_eeprom(frame);
	send_preflight((char *)frame, EEP_FRAME_SIZE);
 
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
