/*
 * GccApplication2.c
 *
 * Created: 15-05-2014 13:42:06
 *  Author: ASHTESH
 */ 


#include <avr/io.h>
#include "common.h"
#include "uart.h"
#include "spi.h"
#include "sun.h"
#include "peripherals.h"
int main(void)
{
   /*char array[40];
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
	sprintf(array2,"\rCurrent Sunsensor state is =\t");
		
/************************************************************/
	char buffer[40];
	double aa;
	int i;
	unsigned int bb;
	init_SPI();
  
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
	PORTA = 0xA0;
	_delay_ms(500);
	PORTA = 0x50;
	_delay_ms(500);
	configure_torquer();   //tor
	///* * Reading Sunsensor
	while(1)
	{
    read_SS();
	//send_preflight((char *)&Current_state.ss, sizeof(struct SS_reading));
	
	//for(i=0;i<6;i++)
	//{ 
		
		aa = Current_state.ss.reading[0];
		
		bb = ((aa-2047)/2047)*65535 ;
		Current_state.pwm.x_dir =0;             //0;
		Current_state.pwm.x = bb ;             //32768; 0.5
		Current_state.pwm.y_dir = 0;            //0;
		Current_state.pwm.y = bb;            //32768; 0.75
		Current_state.pwm.z_dir =0;             // 0;
		Current_state.pwm.z = bb;
		set_PWM ();                                    //bb = (aa*4*100)/4096;
		//sprintf(buffer, "%u\t", bb);
		//send_preflight( buffer, strlen(buffer));
	//}
	
/************************************************************/
	}
}