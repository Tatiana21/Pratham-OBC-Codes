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
 * hMaster_GPStest.c
 *
 * Created: 07-04-2012 04:56:21
 *  Author: Hussain
 */

#include "common.h"
#include "uart.h"
#include "gps.h"
#include "peripherals.h"

/**
 * @defgroup global_state Global State parameters
 */
//@{
volatile int8_t GPS_done = -1;
uint8_t Mode;
uint64_t Time;
volatile struct state Current_state;
//@}

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char arr[40];
	char array1[40];
	char array2[40];
	 /*   Current_state.gps.x = 1;
	    Current_state.gps.y = 1;
	    Current_state.gps.z = 1;
	    Current_state.gps.v_x = 1;
	    Current_state.gps.v_y = 1;
	    Current_state.gps.v_z = 1;
	    Current_state.gps.year = 1;
	    Current_state.gps.alt = 1;
	    Current_state.gps.date = 1;
	    Current_state.gps.hours = 1;
	    Current_state.gps.lat = 1;
	    Current_state.gps.lon = 1;
	    Current_state.gps.minutes = 1;
	    Current_state.gps.month = 1;
	    Current_state.gps.seconds = 1;
	    Current_state.gps.time_since_reading = 1;
*/
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
	sprintf(array2,"\rCurrent GPS state is =\t");
		
/************************************************************/
	init_UART_GPS();
	
	///* Switch on Global interrupts
	sei();
  
/************************************************************/
	while(1){
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	/*transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	transmit_string_UART0(array2);
	
/************************************************************/
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	///* * Reading GPS
    read_GPS();
    while(UCSR0B & _BV(RXCIE0));
    //send_preflight((char *)&Current_state.gps, sizeof(struct GPS_reading));
/*uint32_t c= 0x3F4F04AC;
sprintf(arr,"2%x",c);
transmit_string_UART0(arr);*/
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
