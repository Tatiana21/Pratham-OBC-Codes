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
 * hMaster_Preflighttest.c
 *
 * Created: 13-04-2012 14:12:06
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"
#include "spi.h"
#include "twi.h"
#include "mag.h"
#include "gps.h"
#include "hm.h"
#include "sun.h"
#include "slave_comm.h"
#include "functions.h"
#include "ax25.h"
#include "timer.h"
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


void blink (void)
{
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	
}

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array1[40];
	char array2[40];
	char array3[40];
	
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
	
	sprintf(array1,"\t..This is IITB's Student Satellite...\r");
	transmit_string_UART0(array1);

	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
		
/************************************************************/
	char buffer[40];
	double aa;
	int i;
	unsigned int bb;
	uint8_t frame[100], buf[100], x;
	
	/// Initialise Interfaces - UART of Magnetometer and GPS and the SPI bus
	init_UART_MM();
	_delay_ms(100);
	init_UART_GPS();
	_delay_ms(100);
	init_SPI();
	_delay_ms(100);
	init_TWI();
	_delay_ms(100);
	
	///Configure the Torquer
	configure_torquer();
  	_delay_ms(100);
   
	///Configure CC1020
	init_CC1020();
	
	///Set Preflight pin as input
	cbi(DDR_PF, PIN_PF);
  
	///* Switch on Global interrupts
	sei();
  
/************************************************************/
	while(1)
	{
	blink();
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	
/************************************************************/
	blink();
		
    ///* Preflight Checks
    if(PORT_PF & _BV(PIN_PF))
	{	///* * Set the mode as preflight
		Mode = PREFLIGHT;
    
		sprintf(array2,"\r...Entering PREFLIGHT Mode...\r");
		transmit_string_UART0(array2);
		
		blink();
		
		///* * Reading GPS
	    sprintf(array2,"\rCurrent GPS state is =\t");
		transmit_string_UART0(array2);
		read_GPS();
		while(UCSR0B & _BV(RXCIE0));
		send_preflight((char *)&Current_state.gps, sizeof(struct GPS_reading));

		blink();
	
		/************************************************************/
		///* * Magnetometer and Torquer test

		sprintf(array2,"\rCurrent MagnetoMetr state is =\t");
		sprintf(array3,"\rGenerating Torquer Current for :\t");
		
		///* * Reading with no torquers on
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
		
		blink();
		
		///* * Reading with one torquer on at once, in one direction
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('X');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 32768;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('Y');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 32768;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('Z');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 32768;  
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		///* * Reading with ALL torquer on at once, in one direction
		transmit_string_UART0(array3);
		transmit_UART0('-');
		transmit_UART0('X');
		transmit_UART0(' ');
		transmit_UART0('Y');
		transmit_UART0(' ');
		transmit_UART0('Z');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 32768;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 32768;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 32768;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		///* * Reading with one torquer on at once, in other direction
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('X');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 1;
		Current_state.pwm.x = 32768;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('Y');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 1;
		Current_state.pwm.y = 32768;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		transmit_string_UART0(array3);
		transmit_UART0('+');
		transmit_UART0('Z');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 1;
		Current_state.pwm.z = 32768;  
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		blink();
		
		///* * Reading with ALL torquer on at once, in other direction
		transmit_string_UART0(array3);
		transmit_UART0('-');
		transmit_UART0('X');
		transmit_UART0(' ');
		transmit_UART0('Y');
		transmit_UART0(' ');
		transmit_UART0('Z');
		transmit_UART0('\r');
		Current_state.pwm.x_dir = 1;
		Current_state.pwm.x = 32768;
		Current_state.pwm.y_dir = 1;
		Current_state.pwm.y = 32768;
		Current_state.pwm.z_dir = 1;
		Current_state.pwm.z = 32768;
		set_PWM ();
		blink();
		blink();
		
		transmit_string_UART0(array2);
		read_MM ();
		send_preflight((char *)&Current_state.mm, sizeof(struct MM_reading));
    
		///* * Set Torquer values to zero
		reset_PWM();
  
		blink();
		
		///* * Reading Sunsensor
		sprintf(array2,"\rCurrent SunSensor state is =\t");
		transmit_string_UART0(array2);
		read_SS();
		//send_preflight((char *)&Current_state.ss, sizeof(struct SS_reading));
		for(i=0;i<6;i++)
		{
			aa = Current_state.ss.reading[i];
			bb = (aa*3.3*100)/4096;
			sprintf(buffer, "%u\t", bb);
			send_preflight( buffer, strlen(buffer));
		}
		
		blink();
	
		///* * Reading Slave
		sprintf(array2,"\rCurrent Health state is =\t");
		transmit_string_UART0(array2);
		get_HM_data();
		send_preflight((char *)&Current_state.hm, sizeof(Current_state.hm ));
		
		blink();
	
		///* * Communication Task
		sprintf(array2,"\rCC configured and Data send to be transmitted :\t");
		sprintf(frame, "Hello World");
		x = init_CC1020_TX();
	
		if(x)
		{	sprintf(array3, "Done:%d\tPLL Locked...Transmission Started\r\n", x);
			transmit_string_UART0(array3);
		}
		else
		{	transmit_string_UART0("\tPLL Lock Failed\r\n");
			break;	
		}
		transmit_string_UART0(array2);
		transmit_string_UART0(frame);
	
		slave_send (REAL_TIME, frame, FRAME_SIZE);
		blink();
		
		///* Start transmission
		sprintf(array3,"\rCommand send to Slave :\t%d", BEGIN_TX_COMM);
		transmit_string_UART0(array3);
		slave_send (BEGIN_TX_COMM, NULL, 0);
		blink();
		blink();
		
		///* Stop transmission
		sprintf(array3,"\rCommand send to Slave :\t%d", END_TX);
		transmit_string_UART0(array3);
		slave_send (END_TX, NULL, 0);
		blink();
		
		///* Wait for Acknoledgement
		while(SPI_transfer(0xFF) != ACK);
    
		SetupCC1020PD();  // configure in PD and stop Transmission
	
	}	
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/

