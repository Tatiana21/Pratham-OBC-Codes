/****************************************************************************/
/* 					PRATHAM - IITB's Student Satellite                      */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Atmel AVRmega8L                                               */
/* Written for the AVRStudio5 compiler                                      */
/*                                                                          */
/* Author:  Hussain Manasawala, MTech in Electronic Systems, IITB           */
/*                                                                          */
/* Contact: husainmanasa@ee.iitb.ac.in                                      */
/*                                                                          */
/****************************************************************************/
/*
 * hBeacon_OOKtest.c
 *
 * Created: 01-04-2012 01:14:36
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	/*char array[40];
	char array1[40];
	char array2[50];

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
	
	sprintf(array1,"\tThis is HUSSAIN's Beacon code...");
	sprintf(array2,"\rTransmitting...\t'PRATHAMIITBOMBAYSTUDENTSATELLITE'\r");*/

	//The morse code will be generated at pin PC0 (Physical pin 23) of the Atmega8.
		
/************************************************************/
	_delay_ms(1000);
	
	DDR_PA = 0x01;
	PORT_PA = 0x00;
	
	while(1){
	
	//transmit_string_UART0(array1);
	//transmit_string_UART0(array2);

/************************************************************/
        int len1 = 35;
        int len3 = len1 * 3;
	
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);

		//For R
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For A
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For H
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For A
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For M
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For I
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For I
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For B
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		//For O
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For M
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For B
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		//For A
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For Y
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For S
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For U
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For D
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For E
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For N
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For S
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);	
		_delay_ms(len3);
		//For A
		cbi(PORT_PA,PA_EN);	
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For E
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For L
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For L
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For I
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For T
		cbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len3);
		//For E
		cbi(PORT_PA,PA_EN);
		_delay_ms(len1);
		sbi(PORT_PA,PA_EN);
		_delay_ms(len1 * 7);

/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
