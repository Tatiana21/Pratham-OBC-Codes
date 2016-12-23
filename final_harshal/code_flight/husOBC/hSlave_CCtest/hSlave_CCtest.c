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
 * hSlave_CCtest.c
 *
 * Created: 18-04-2012 22:01:51
 *  Author: Hussain
 */ 

#include "common.h"
#include "ax25.h"
#include "receive.h"
#include "transmit.h"
#include "functions.h"
#include "spi.h"
#include "uart.h"

#define CC1020_TX			// Define as CC1020_TX to configure as Transmitter
//#define CC1020_RX			// Define as CC1020_RX to configure as Receiver

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
	
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
	
	sprintf(array,"\tThis is HUSSAIN's OBC-Slave code...");
	sprintf(array2,"\rWireless Communication Using CC1020\r");
		
/************************************************************/

	uint8_t frame[100], buf[100], x;

	init_SPI();
	//init_SPI_trans(CC1020);  
	_delay_ms(1000);
   
	cc1020_init_dio();
	init_CC1020();
	
	DumpCC1020Regs();
  
	#ifdef CC1020_TX

/**********		Start Transmission		**********/
	sprintf(frame, "Hello World");
	sprintf(buf, "Number");
	sprintf(TXBUFFER, "HelloWorld ImPRATHAM");
	
 	//sbi(DDR_PA, PA_EN);		
	//sbi(PORT_PA, PA_EN);		//Enable PA
	
	while(1){
	_delay_ms(1000);
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array);
	transmit_string_UART0(array2);

/************************************************************/
	x = init_CC1020_TX();
	
	if(x)
	{	sprintf(array1, "%d Done:\tPLL Locked\tTransmitting..\r\n", x);
			transmit_string_UART0(array1);
	}
	else
	{	transmit_string_UART0("\tPLL Lock Failed\r\n");
		break;	
	}
		
	make_ax25_frame_from_data(frame, buf);
	//cc1020_transmit_packet(frame,FRAME_SIZE);
	
	cc1020_transmit_rf_packet(TXBUFFER);
	
	//if(x)
	//{
	//	sprintf((char *)frame,"\rTransmission complete; No. of bytes send = \t%d\r", x);
		transmit_string_UART0("\rTransmission complete\r");
	//	transmit_string_UART0(frame);
	//}
	SetupCC1020PD();  // configure in PD and stop Transmission
	}	
/**********		End of Transmission		**********/

	#else

/**********		Start Reception		**********/
 
	//sbi(DDR_LNA, LNA_EN);		//Enable LNA

	while(1){
	_delay_ms(1000);
	
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('i');
	
	transmit_string_UART0(array1);
	transmit_string_UART0(array2);

/************************************************************/
	x = init_CC1020_RX();
	
	if(x)
	{	sprintf(array1, "%d Done:\tPLL Locked\r\n", x);
		transmit_string_UART0(array1);
	}
	else
	{	transmit_string_UART0("\tPLL Lock Failed\r\n");
		break;	
	}
		
	//x = cc1020_receive_packet(frame);
	cc1020_receive_rf_packet(RXBUFFER);
	//if(x)
	//{
		sprintf(array1,"\rReception complete; No. of bytes received :\t%d\r", x);
		transmit_string_UART0(array1);
	//}

	SetupCC1020PD();  // Do not configure in PD and stop Reception			
	}		

/**********		End of Reception		**********/
	#endif

	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/

// Dumps the values of all the registers of the CC1000 to the screen
void DumpCC1020Regs(void)
{
	unsigned char RegCounter;

	transmit_string_UART0("Register dump:\n\r\n\r");
	for(RegCounter=0x00; RegCounter<=0x20; RegCounter++)
    {	transmit_string_UART0("\rAddr. ");
		transmit_ASCII_UART0(RegCounter);
		transmit_string_UART0(" : ");
		transmit_ASCII_UART0(ReadFromCC1020Register(RegCounter));
	}
}
