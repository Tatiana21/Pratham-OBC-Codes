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
 * hMaster_MMtest.c
 *
 * Created: 31-03-2012 15:04:56
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"
#include "mag.h"
#include "peripherals.h"

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
		 int16_t Bx;
		 int16_t By;
		 int16_t Bz;
	
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
	int16_t a=1500;
	sprintf(array,"%d",a);
	transmit_string_UART0(array);

	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array2,"\rCurrent MagnetoMetr state is =\t");
		
/************************************************************/
	init_UART_MM();
	
/************************************************************/
/*	while(1){
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
	
/************************************************************/
	//transmit_string_UART1("*00P\r");
	/*PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
	///* * Reading with no torquers on
    read_MM ();*/

	char sx[20];
	char sy[20];
	char sz[20];
	
	   while (1){
	 //  transmit_UART0('d');
	   send_MM_cmd("*00P\r");
			 Bx=(int16_t)receive_MM();
		     Bx=(Bx<<8); 
			 Bx &= 0xFF00;
		     Bx|=(int16_t)receive_MM();
			 transmit_UART0('x');
			 By=(int16_t)receive_MM();
			 By=(By<<8); By &= 0xFF00;
			 By|=(int16_t)receive_MM();
			 transmit_UART0('y');		 
			 Bz=(int16_t)receive_MM();
			 Bz=(Bz<<8); Bz &= 0xFF00;
			 Bz|=(int16_t)receive_MM();
			 transmit_UART0('x');
		     receive_MM();
			 transmit_UART0('\r');
	 sprintf(sx,"%d",Bx);
	 sprintf(sy,"%d",By);
	 sprintf(sz,"%d",Bz);
	 transmit_string_UART0(sx);
	 transmit_UART0(' ');
	 transmit_string_UART0(sy);
	 transmit_UART0(' ');
	 transmit_string_UART0(sz);
	 transmit_UART0('\r');
	// _delay_ms(1000);
   }
	   
}
   
	

    //send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
/************************************************************/
	//}
	//return 0;
//}
/************************************************************/
/*				Main ends									*/
/************************************************************/
