/*1mod
 * sun.c
 *
 * Created: 31-03-2012 15:29:04
 *  Author: Hussain
 */ 

/**
 * @file sun.c
 * @brief Read the ADC values for the Sun sensors
 */

#include "common.h"
#include "sun.h"
#include "spi.h"
#include "peripherals.h"

void configure_SS(void)
{
	///Set PD and Range pins as output
	sbi(DDRB, PB6);
	sbi(DDRB, PB7);
  
	///Set PD high: power up the ADC IC
	sbi(PORTB, PB6);
	///Set Range high: Selecting range -> Range2
	sbi(PORTB, PB7);
  
	_delay_us(10);
	///Configure GPIO pins - GPIO3 as PD and GPIO2 as Range input
	init_SPI_trans(ADC_S);
	SPI_transfer(0b01000001);// ankri bit 1
	SPI_transfer(0b00000000);
	end_SPI();
	_delay_us(10);
}

void poll_SS(void)
{
  uint8_t channel = 0;
  
  /*while(channel < 1)
  {
      cbi(PORTB, ADC_S);
    Current_state.ss.reading[channel] = SPI_transfer(0x00);
    Current_state.ss.reading[channel] = Current_state.ss.reading[channel] << 8;
    Current_state.ss.reading[channel] &= 0x0F00;
     Current_state.ss.reading[channel] |= SPI_transfer(0x00);
    channel++;
    sbi(PORTB, ADC_S);
}
///Power Down PD low
  cbi(PORTB, PB6);
  return;*/
  
  ///Loop for reading the 6 sun sensor values
  while(channel <= 5)
  {
    ///* Put the ADC reading in the appropriate variable
    /*init_SPI_trans(ADC_S);
    SPI_transfer(0b00011000 | (channel >> 1));
    SPI_transfer(((channel & 0x01) << 7) | 0x40);
	end_SPI();
	_delay_us(1);
    
    init_SPI_trans(ADC_S);
    SPI_transfer(0b00011000 | (channel >> 1));
    SPI_transfer(((channel & 0x01) << 7) | 0x40);
    end_SPI();
	_delay_us(1);
    */
    init_SPI_trans(ADC_S);
    Current_state.ss.reading[channel] = SPI_transfer(0b00011000 | (channel >> 1));;
    Current_state.ss.reading[channel] = Current_state.ss.reading[channel] << 8;
    Current_state.ss.reading[channel] &= 0x0F00;
    Current_state.ss.reading[channel] |= SPI_transfer(((channel & 0x01) << 7) | 0x50);//0x40
    end_SPI();
	_delay_us(10);
    
   channel++;
    
  }
  ///Power Down PD low
  cbi(PORTB, PB6);
}
