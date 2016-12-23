/*
 * mag.c
 *
 * Created: 31-03-2012 15:15:08
 *  Author: Hussain
 */ 

/**
 * @file mag.c
 * @brief Read the magnetic field vector from the Magnetometer
 */
#include "common.h"
#include "mag.h"
#include "peripherals.h"
#include "uart.h"


void init_UART_MM(void )
{

  UCSR1A = 0;
  UCSR1B = 0;
  UCSR1C = 0;
  
  ///Double Baud Rate
  UCSR1A |= _BV(U2X1);
  ///Enable Transmission and Reception
  UCSR1B |= _BV(TXEN1) | _BV(RXEN1) |  _BV(RXCIE1);
  ///8-bit Data Byte
  UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);
  ///Set Baud Rate to 9600
  UBRR1L = 103;
  UBRR1H = 0;
}

static volatile uint8_t t;

void poll_MM(void)
{
  ///Temporary variables for magnetometer readings
  int16_t x, y, z;
  uint8_t tmp;
  
  ///Send the poll command
  send_MM_cmd("*00P\r");
  
  x = ((int16_t)receive_MM()) << 8;
  x &= 0xFF00;
  x |= (int16_t)receive_MM();
  
  
  y = ((int16_t)receive_MM()) << 8;
  y &= 0xFF00;
  y |= (int16_t)receive_MM();
  
  z = ((int16_t)receive_MM()) << 8;
  z &= 0xFF00;
  z |= (int16_t)receive_MM();
  
  receive_MM();
  
  char buf[100];
  sprintf(buf,"%d %d %d\r",  x, y, z);
  send_preflight(buf, strlen(buf));
    
  ///Convert the readings to Gauss
  Current_state.mm.B_x = ((float) x) / 15000;
  Current_state.mm.B_y = ((float) y) / 15000;
  Current_state.mm.B_z = ((float) z) / 15000;
}

uint8_t receive_MM(void)
{
  ///Wait for a Byte to arrive
  while (!(UCSR1A & _BV(RXC1)));
  return UDR1; 
}

void send_MM_cmd(char *data)
{
  ///Send the char array till stop
  while((*data) != '\0')
  {
    ///* Wait for empty transmit buffer 
    while ( !(UCSR1A & (_BV(UDRE1))) );
    ///* Start transmission
    UDR1 = *data;
    data++;
  }
}

