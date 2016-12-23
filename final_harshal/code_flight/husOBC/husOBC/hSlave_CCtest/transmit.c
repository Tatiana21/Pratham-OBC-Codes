/*
 * transmit.c
 *
 * Created: 18-04-2012 01:40:16
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"
#include "transmit.h"

void cc1020_transmit_bit(uint8_t bit)
{
  while(PIN_TRX & (1 << DCLK));
  if(bit == 0)
    cbi(PORT_TRX, DIO);
  else
    sbi(PORT_TRX, DIO);
  while(!(PIN_TRX & (1 << DCLK)));
}

//with bit stuffing
void cc1020_transmit_packet(uint8_t *data, uint8_t size)
{
  uint8_t i, j, byte, bit;
  uint8_t ax_counter = 0;
  
  cc1020_transmit_bit(1);
  
  for(i = 0; i < size; i++)
  {
    byte = data[i];
    for(j = 0; j < 8; j++)
    {
      bit = byte & 0x80;
      cc1020_transmit_bit(bit);
      
      if((i != 0) && (i != size - 1))
      {
        if(bit)
          ax_counter++;
        else
          ax_counter = 0;
        if(ax_counter == 5)
        {
          ax_counter = 0;
          cc1020_transmit_bit(0);
        }
      }  
      
      byte = byte << 1;
    }
  }
  
  sbi(PORT_TRX, DIO);
}


/************************************************************************/
/************************************************************************/
char cc1020_transmit_rf_char(uint8_t trans_data)
{
	int BitCounter;
	char tempData;
	tempData = trans_data;
	for (BitCounter = 0; BitCounter < 8; BitCounter++)
	{
		while (!(PIN_TRX & (1 << DCLK)));	// put new data on falling edge of DCLK
		while (PIN_TRX & (1 << DCLK));			// since by then it is stable

			
		if (( tempData & 0x80)==0 )	
			cbi(PORT_TRX, DIO); // DIO=0
		else 
			sbi(PORT_TRX, DIO); //	DIO=1
		
		tempData = tempData<<1;		
		transmit_ASCII_UART0(tempData);
	}// end for loop
}

void cc1020_transmit_rf_packet(uint8_t *trans_buffer) 
{
	short i;

	cc1020_transmit_rf_char(0xFF);
	cc1020_transmit_rf_char(0xFF);
	cc1020_transmit_rf_char(0xFE);
	cc1020_transmit_rf_char(rf_lock1);
	cc1020_transmit_rf_char(rf_lock2);
//	msg_len = packet.data_pack.common_header.msg_len;
//	for (i=0; i<msg_len ; i++ )	{
	for (i=0; i<20 ; i++ )	
	{
		cc1020_transmit_rf_char(trans_buffer[i]);
	}
}
