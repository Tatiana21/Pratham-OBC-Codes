/*
 * gps.c
 *
 * Created: 31-03-2012 15:30:41
 *  Author: Hussain
 */ 

/**
 * @file gps.c
 * @brief Read the GPS data using a UART interrupt
 */

#include "common.h"
#include "peripherals.h"
#include "propagator.h"
#include "gps.h"

///4-byte buffer for the GPS reading
volatile static uint32_t buffer = 0;
///Position variables for the data in GPS structure
volatile static uint8_t pos = 0xFF,vel = 0xFF,dop = 0xFF,geo = 0xFF, time = 0xFF;
///Variables to check whether the message has ended
volatile static uint8_t last_byte, message_end;
volatile static uint16_t pdop = 0xFFFF;

///Temporary GPS reading
volatile struct GPS_reading gps;

void init_UART_GPS(void )
{

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  
  ///Double Baud Rate
  UCSR0A |= _BV(U2X0);
  ///Enable Reception
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
  ///8-bit Data Byte, 2 Stop bits
  UCSR0C |= _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
  ///Set Baud Rate to 9600
  UBRR0L = 103;
  UBRR0H = 0;
}

/** @brief Interrupt on receiving a byte through UART GPS
 */
ISR(USART0_RX_vect)
{
  char array[40];
  char arrayy[40];
  char arrayx[40];
  char arrayz[40];
  ///Buffer the Received Byte
  last_byte = UDR0;
  
  ///Put the received byte in the last 4-bytes buffer
  buffer = buffer << 8;
  buffer &= 0xFFFFFF00;
  buffer |= (uint32_t) last_byte;
  
  ///Check if the last byte was for position
  if(pos <12)
  {
    
    ///* Put the data into the structure
   // *((uint8_t *)&gps.x + pos) = last_byte;
   // sprintf(array,"%x",last_byte);
	//transmit_string_UART0(array);
    ///* Increment position and terminate it if full
	if(pos<4)
	{
		
		Current_state.gps.x = Current_state.gps.x << 8;
		Current_state.gps.x &= 0xFFFFFF00;
		Current_state.gps.x |= (uint32_t) last_byte;
		/*sprintf(arrayx,"%x %x",last_byte);transmit_string_UART0(arrayx);
		transmit_UART0('\r');*/
	}
	else if (pos<8)
	{
		Current_state.gps.y = Current_state.gps.y << 8;
		Current_state.gps.y &= 0xFFFFFF00;
		Current_state.gps.y |= (uint32_t) last_byte;
		/*sprintf(arrayy,"%x %x",last_byte);transmit_string_UART0(arrayy);
		transmit_UART0('\r');*/
	}
	
	else //if(pos<12)
	{
		Current_state.gps.z = Current_state.gps.z << 8;
		Current_state.gps.z &= 0xFFFFFF00;
		Current_state.gps.z |= (uint32_t) last_byte;
		/*sprintf(arrayz,"%x %x",last_byte);transmit_string_UART0(arrayz);
		transmit_UART0('\r');*/
	}
   pos++;
  }  //ch2*/
     if(pos==12)
         {
		  pos = 0xFF;
		  sprintf(arrayx,"%x %x",Current_state.gps.x);transmit_string_UART0(arrayx);
		  transmit_UART0('\r');
		  sprintf(arrayy,"%x %x",Current_state.gps.y);transmit_string_UART0(arrayy);
		  transmit_UART0('\r');
		  sprintf(arrayz,"%x %x",Current_state.gps.z);transmit_string_UART0(arrayz);
		  transmit_UART0('\r');
		  Current_state.gps.x=0;
		  Current_state.gps.y=0;
		  Current_state.gps.z=0;
		  buffer=0;
	     }
  
  ///Check if the last byte was for velocity
 /* if(vel < 12)
  {
    *((uint8_t *)&gps.v_x + vel) = last_byte;
    
    vel++;
    if(vel == 12)
      vel = 0xFF;
  }
  
  ///Check if the last byte was for PDOP
  if(dop < 4)
  {
    if(dop >= 2)
      *((uint8_t *)&pdop + (dop - 2)) = last_byte;
      
    dop++;
    if(dop == 4)
      dop = 0xFF;
  }
  
  ///Check if the last byte was for Geodetic position
  if(geo < 16)
  {
    if(geo >= 4)
    *((uint8_t *)&gps.lat + (geo - 4)) = last_byte;
    
    geo++;
    if(geo == 16)
      geo = 0xFF;
  }
  
  ///Check if the last byte was for Time
  if(time < 7)
  {
    *((uint8_t *)&gps.hours + time) = last_byte;
    
    time++;
    if(time == 7)
    {
      time = 0xFF;
      ///* * The Entire message has been read
      message_end = 1;
    }
  }
  */
  ///Check the buffer against message ID's
  ///Position
  if(buffer == 0x3F3F04AC)
  {
	sprintf(array,"%x %x",buffer);
	transmit_string_UART0(array);
	transmit_UART0('\r');
    message_end = 0;
    pos = 0;
  }
  ///Velocity
 /* else if(buffer == 0x3F3F05AC)
    vel = 0;
  ///DOP
  else if(buffer == 0x3F3F0BAC)
    dop = 0;
  ///Geodetic System
  else if(buffer == 0x3F3F0EAC)
    geo = 0;
  ///Time
  else if(buffer == 0x3F3F0FAC)
    time = 0;
  
  ///Check if PDOP < 500(factor 100), shut off the interrupt and the GPS device
  if((pdop < 500) && (pdop!=0) && message_end)
  {
    GPS_done = 1;
    
    ///* Switch off the interrupt
    UCSR0B &= ~(_BV(RXCIE0));   
    
    ///* Switch off the GPS device
    //power_down_peripheral(PGPS);
    
    pdop = 0xFFFF;
    message_end = 0;
    buffer = 0;
  }*/
  return 0;//ch1
}
