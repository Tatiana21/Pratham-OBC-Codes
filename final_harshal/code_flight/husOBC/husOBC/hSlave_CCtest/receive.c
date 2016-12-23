/*
 * receive.c
 *
 * Created: 01-04-2012 03:22:42
 *  Author: Hussain
 */ 

#include "common.h"
#include "uart.h"
#include "receive.h"
#include "ax25.h"

//conformation to standard : CRC and transmission sequence: least/most significant bit
//bit stuffing

uint8_t cc1020_receive_bit(void)
{
  uint8_t bit;
  
  while(!(PIN_TRX & (1 << DCLK)));
  bit = PIN_TRX & (1 << DIO);
  while(PIN_TRX & (1 << DCLK));
  
  return bit;
}

uint8_t cc1020_receive_packet(uint8_t *data)
{
  static uint8_t byte = 0;
  uint8_t i, bit, size = 0, flag = 0;
  uint8_t ax_counter = 0;
  
  uint16_t crc_in_packet, crc;
  
  while(byte != AX_FLAG)
  {
    bit = cc1020_receive_bit();
    byte = byte << 1;
    if(bit == 0)
      byte &= ~(0x01);
    else
      byte |= 0x01;
  }
  
  do
  {
    for(i = 0; i < 8; i++)
    {
      bit = cc1020_receive_bit();
      byte = byte << 1;
      if(bit == 0)
        byte &= ~(0x01);
      else
        byte |= 0x01;
      
      if(bit)
      {
        ax_counter++;
        if(ax_counter == 5)
        {
          ax_counter = 0;
          bit = cc1020_receive_bit();
          if(bit)
          {
            byte = byte << 1;
            byte |= 0x01;
            
            flag = 1;
            
            if(i == 5)
              i++;
            else
              return 0;
          }
        }
      }
      else
        ax_counter = 0;
    }
    data[size++] = byte;
    
  }while(!flag);
  
  if(size < 4)
    return 0;
  
  memcpy(&crc_in_packet, data + size - 3, sizeof(uint16_t));
  crc = calculate_crc_16(data, size - 3);
  if (crc_in_packet == crc)
    return size;
  
  //send_UART(data, size);
  //send_UART("CRC fail\r", 9);
  //return 0;
}


/************************************************************************/
/************************************************************************/
char cc1020_receive_rf_char(void)
{
	int i;
	char ShiftReg = 0x00;
	for ( i=0 ; i<8 ; i++) {
		while(PIN_TRX & (1 << DCLK));			//sample data on rising edge of DCLK
		while(!(PIN_TRX & (1 << DCLK)));
		ShiftReg = ShiftReg << 1;
		if ((PIN_TRX & (1 << DIO)) == 0)
			ShiftReg|=0x01;
		else
			ShiftReg&=0xFE;
		transmit_ASCII_UART0(ShiftReg);
	}
	return ShiftReg;
}

char cc1020_receive_rf_packet(uint8_t *recv_buffer) 
{
	//char recv_buffer[20];
	enum boolean FrameDetected, UnLock;
	short int i;

	i = 0;
	new_packet_received = FALSE;
	FrameDetected = FALSE;
	UnLock = FALSE;

	do {
		while(PIN_TRX & (1 << DCLK));			//sample data on rising edge of DCLK
		while(!(PIN_TRX & (1 << DCLK)));
	
		if ((PIN_TRX & (1 << DIO)) == 0) {
			if (i < 20) 
				i++;
		} 
		else {
			if (i >= 16) {
				FrameDetected = TRUE;
			}
			else
				i = 0;
		}
	} while (FrameDetected == FALSE);	//leave the loop only when frame is detected
// start bit detected
	rf_key1 = cc1020_receive_rf_char();
	rf_key2 = cc1020_receive_rf_char();

	if (rf_key1==rf_lock1 && rf_key2==rf_lock2) 
	{
		cli();
		for (i=0; i<20 ; i++) 
		{
			recv_buffer[i] = cc1020_receive_rf_char();
		}
		sei();
		new_packet_received = TRUE;		
	}
	return (recv_buffer[i-1]);
} // receive_RF_packets ends here

