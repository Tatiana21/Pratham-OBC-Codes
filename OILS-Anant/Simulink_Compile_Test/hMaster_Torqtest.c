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

	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array2,"\rGenerating Torquer Current for =\t");
		
/************************************************************/
	configure_torquer();
	
/************************************************************/
	while(1){              //     CH1
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	_delay_ms(1000);
	
	//begin OILS code to test whether matlab code compiles
	
	int size_mag = 0;
	int size_sun = 0;
	int size_gps = 0;
	uint8_t mag[size_mag];
	uint8_t sun[size_sun];
	uint8_t gps[size_gps];
	
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
	
	/*transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	
/************************************************************/
	///* * Magnetometer and Torquer test

    ///* * Reading with ALL torquer on at once, in one direction
    /*transmit_string_UART0(array2);
	transmit_UART0('+');
	transmit_UART0('X');
	transmit_UART0(' ');
	transmit_UART0('Y');
	transmit_UART0(' ');
	transmit_UART0('Z');
	transmit_UART0('\r');*/
	//int8_t myhead = receive_UART0();
	//if (myhead == 97)
	//{
		//init_UART0();
		//int8_t a = receive_UART0();
		//transmit_UART0(a);
		//int8_t b = receive_UART0();
		//transmit_UART0(b);
		int16_t c = 32768;//((int16_t)(b<<8)|(int16_t)a);
		//transmit_UART0((uint8_t)c);
		//transmit_UART0((uint8_t)(c>>8));
		Current_state.pwm.x_dir =0;             //0;
		Current_state.pwm.x =c;             //32768; 0.5
		Current_state.pwm.y_dir = 0;            //0;
		Current_state.pwm.y = c;            //32768; 0.75
		Current_state.pwm.z_dir =0;             // 0;
		Current_state.pwm.z = c;                //0.25
		set_PWM ();
	//}
    /*       CH2
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	
	///* * Reading with one torquer on at once, in one direction
    transmit_string_UART0(array2);
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
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	
    transmit_string_UART0(array2);
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
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	
    transmit_string_UART0(array2);
	transmit_UART0('+');
	transmit_UART0('Z');
	transmit_UART0('\r');
	Current_state.pwm.x_dir = 0;
    Current_state.pwm.x = 0;
    Current_state.pwm.y_dir = 0;
    Current_state.pwm.y = 0;
    Current_state.pwm.z_dir = 0;
    Current_state.pwm.z = 32768;  
    set_PWM ();
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	
	///* * Reading with ALL torquer on at once, in other direction
    transmit_string_UART0(array2);
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
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	
	///* * Reading with one torquer on at once, in other direction
    transmit_string_UART0(array2);
	transmit_UART0('-');
	transmit_UART0('X');
	transmit_UART0('\r');
    Current_state.pwm.x_dir = 1;
    Current_state.pwm.x = 32768;
    Current_state.pwm.y_dir = 0;
    Current_state.pwm.y = 0;
    Current_state.pwm.z_dir = 0;
    Current_state.pwm.z = 0;
    set_PWM ();
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
  
    transmit_string_UART0(array2);
	transmit_UART0('-');
	transmit_UART0('Y');
	transmit_UART0('\r');
    Current_state.pwm.x_dir = 0;
    Current_state.pwm.x = 0;
    Current_state.pwm.y_dir = 1;
    Current_state.pwm.y = 32768;
    Current_state.pwm.z_dir = 0;
    Current_state.pwm.z = 0;
    set_PWM ();
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	  
    transmit_string_UART0(array2);
	transmit_UART0('-');
	transmit_UART0('Z');
	transmit_UART0('\r');
	Current_state.pwm.x_dir = 0;
    Current_state.pwm.x = 0;
    Current_state.pwm.y_dir = 0;
    Current_state.pwm.y = 0;
    Current_state.pwm.z_dir = 1;
    Current_state.pwm.z = 32768;  
	set_PWM ();
    
	PORTA = 0xA0;
	_delay_ms(10000);
	PORTA = 0x50;
	_delay_ms(10000);
	  
	///* * Set Torquer values to zero
    reset_PWM();
  
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	*/
/************************************************************/
	}//CH3
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
