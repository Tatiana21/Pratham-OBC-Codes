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
 * hSlave_MasterSPItest.c
 *
 * Created: 08-04-2012 01:26:24
 *  Author: Hussain
 */ 

#include "common.h"
#include "ax25.h"
#include "slave_comm.h"
#include "spi.h"
#include "uart.h"

//Variables required for receiving data thru the SPI interface
volatile char message[256];
volatile uint8_t end_spi = 0, start_spi = 0, t = 0, process = 0, spi;

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
	sprintf(array2,"\rData received from Master :\t");
		
/************************************************************/
	uint8_t command;
	uint16_t crc, recv_crc;
	///Initialise SPI as slave
	init_SPI_slave();
	
	///Enable Global Interrupts
	sei();
  
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
	
	if(process)
    {
      crc = calculate_crc_16((uint8_t *)message, t - (end_spi + sizeof(uint16_t)));
      memcpy((void *)&recv_crc, (void *)&(message[t - (end_spi + sizeof(uint16_t))]), sizeof(uint16_t));
      if(crc == recv_crc)
      {
        command = message[0];
        if(command == BEGIN_TX_COMM)
        {
          send_preflight((char *)message + 2, message[1]);
        }
        else if(command == END_TX)
        {
          SPI_transfer(ACK);
        }
      }
      end_spi = 0;
      start_spi = 0;
      t = 0;
      process = 0;
    }
       
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/


/**
 * @brief Interrupt service routine for SPI
 */
ISR(SPI_STC_vect)
{
  spi = SPDR;
  
  if((!process) && (start_spi >= N_END_SPI))
  {
    message[t] = spi;
    
    if(message[t] == END_SPI)
      end_spi++;
    else if(end_spi)
      end_spi = 0;
      
    if(end_spi == N_END_SPI)
      process = 1;
    t++;
  }
  
  if(spi == START_SPI)
    start_spi++;
  else if(start_spi < N_END_SPI)
    start_spi = 0;
}

