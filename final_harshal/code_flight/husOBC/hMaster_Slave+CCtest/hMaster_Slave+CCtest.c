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
 * hMaster_Slave_CCtest.c
 *
 * Created: 14-04-2012 02:49:54
 *  Author: Hussain
 */ 

#include "common.h"
#include "ax25.h"
#include "uart.h"
#include "spi.h"
#include "functions.h"
#include "slave_comm.h"
#include "peripherals.h"

/************************************************************/
/*				Main begins									*/
/************************************************************/
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
  
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('e');
	transmit_UART0('l');
	transmit_UART0('l');
	transmit_UART0('o');
	
	sprintf(array,"\t..This is IITB's Student Satellite...\r");
	transmit_string_UART0(array);

	sprintf(array,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array1,"\rCC configured and Data send to be transmitted :\t");
		
/************************************************************/
	
	uint8_t frame[100], buf[100], x;
	
	init_SPI();
	_delay_ms(1000);
   
	init_CC1020();
	
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
	
	transmit_string_UART0(array);
	
/************************************************************/
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	/**********		Start Transmission		**********/
	sprintf(frame, "Hello World");
	x = init_CC1020_TX();
	
	if(x)
	{	sprintf(array2, "%d Done:\tPLL Locked\r\n", x);
			transmit_string_UART0(array2);
	}
	else
	{	transmit_string_UART0("\tPLL Lock Failed\r\n");
		break;	
	}
	///Communication Task
	transmit_string_UART0(array1);
	transmit_string_UART0(frame);
	
	slave_send (REAL_TIME, frame, FRAME_SIZE);
    _delay_ms(2000);
	
	///* Start transmission
	sprintf(array2,"\rCommand send to Slave :\t%d", BEGIN_TX_COMM);
	transmit_string_UART0(array2);
	slave_send (BEGIN_TX_COMM, NULL, 0);
    _delay_ms(4000);
	
	///* Stop transmission
	sprintf(array2,"\rCommand send to Slave :\t%d", END_TX);
	transmit_string_UART0(array2);
	slave_send (END_TX, NULL, 0);
    _delay_ms(2000);
	  
	///* Wait for Acknoledgement
	while(SPI_transfer(0xFF) != ACK);
    
	SetupCC1020PD();  // configure in PD and stop Transmission
	  
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
