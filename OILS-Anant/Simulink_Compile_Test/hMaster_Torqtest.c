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
 * hMaster_Torqtest.c
 *
 * Created: 07-04-2012 21:00:48
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"
#include "mag.h"
#include "peripherals.h"


int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
	
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
  
	
	configure_torquer();
	
	int size_mag = 0;
	int size_sun = 0;
	int size_gps = 0;
	uint8_t mag[size_mag];
	uint8_t sun[size_sun];
	uint8_t gps[size_gps];
	
	while(1){              
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	_delay_ms(1000);
	
	//begin OILS code to test whether matlab code compiles
	init_UART0();
	
	for(int i=0; i<size_mag; i++){
		mag[i] = receive_UART0();
	}
	
	for(int i=0; i<size_sun; i++){
		sun[i] = receive_UART0();
	}
	
	for(int i=0; i<size_gps; i++){
		gps[i] = receive_UART0();
	}
	
	transmit_UART0((uint8_t)0);
	
	
	}
	return 0;
}

