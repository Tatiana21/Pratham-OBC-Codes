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
  ///Choose the ADC IC on the SPI bus
  init_SPI_trans(ADC_S);
  
  ///Set PD and range pins as input
  cbi(DDRB, PB6);
  cbi(DDRB, PB7);
  
  ///Set PD high: power up the ADC IC
  sbi(PORTB, PB6);
  
  ///Configure GPIO pins - GPIO3 as PD and GPIO2 as range input
  SPI_transfer(0b01000001);
  
  SPI_transfer(0b10000000);
  
  ///Configure Auto-2 Mode
  
  ///Program the Program register of Auto-2 Mode:
  
  SPI_transfer(0b10010001);
  
  SPI_transfer(0b01000000);
  
  ///Set Mode to Auto-2
  
  SPI_transfer(0b00111100);
  
  SPI_transfer(0b00000000);
}

void transmitSunSensorUart(int temp) { //Function for Sending Sunsensor data through UART
	transmit_UART0(temp & 0xff);
	transmit_UART0(temp >> 8);
}

uint16_t convert(uint8_t vall, uint8_t valh) //to convert 2 8 bit integer values to one 16 bit value
{
	
	uint16_t val= valh;
	val=(val<<8)+vall;
	return val;
}

void poll_SS(void)
{
	//***************ADC Variables************************
	uint8_t valh1;//High byte of first sunsensor
	uint8_t vall1;//Low byte of first sunsensor
	uint8_t valh2;
	uint8_t vall2;
	uint8_t valh3;
	uint8_t vall3;
	uint8_t valh4;
	uint8_t vall4;
	uint8_t valh5;
	uint8_t vall5;
	uint8_t valh6;
	uint8_t vall6;

	uint16_t val[6];//First sunsensor combined 16 bit value
	int8_t data_r;
  PORTB |= (1<<PB0);//Set Slave select of Slave OBC to 1
  PORTB &= ~(1<<PB5);// Set slave select of ADC to 0
  //SPCR |=(1<<SPE);
  _delay_ms(1);
  data_r=SPI_transfer(0xFF); //Dummy transaction. Tell ADC that data transmission should begin now.
  _delay_ms(1);
  //transmit_UART0(data_r);
  PORTA = 0xFF;
  vall1=SPI_transfer(0xFF);//First data byte
  _delay_ms(1);
  valh1=SPI_transfer(0xFF);
  _delay_ms(1);


  vall2=SPI_transfer(0xFF);
  _delay_ms(1);
  valh2=SPI_transfer(0xFF);
  _delay_ms(1);




  vall3=SPI_transfer(0xFF);
  _delay_ms(1);
  valh3=SPI_transfer(0xFF);
  _delay_ms(1);



  vall4=SPI_transfer(0xFF);
  _delay_ms(1);
  valh4=SPI_transfer(0xFF);
  _delay_ms(1);



  vall5=SPI_transfer(0xFF);
  _delay_ms(1);
  valh5=SPI_transfer(0xFF);
  _delay_ms(1);



  vall6=SPI_transfer(0xFF);
  _delay_ms(1);
  valh6=SPI_transfer(0xFF);
  _delay_ms(1);
  val[0]=convert(vall1,valh1);
  val[1]=convert(vall2,valh2);
  val[2]=convert(vall3,valh3);
  val[3]=convert(vall4,valh4);
  val[4]=convert(vall5,valh5);
  val[5]=convert(vall6,valh6);

  for (int i=0;i<6;i=i+1)
  {
	  Current_state.ss.read[i] = (double)(((double)val[i]*3.3/1024.0));//Current_state.ss.read[i] = (double)(((double)Current_state.ss.reading[i]*3.3/1024)-1.65)*2;
  }
  
/*  uint8_t sen;
  transmit_UART0('A');
  for(int i=0;i<6;i=i+1)
  {
	  sen = (Current_state.ss.read[i]*255/3.3);
	  transmit_UART0(sen);
  }
 */
  PORTB |= (1<<PB5);
  //_delay_ms(1000);
  ///Power Down PD low
  //cbi(PORTB, PB6);
//return;  
}

// for oils purpose
	
void poll_SS1(void)
{
  uint8_t channel = 0;
  //uint8_t c= 100;
  ///Loop for reading the 6 sun sensor values
  //transmit_UART0(c);
 
  while(channel <= 5)
  {
    ///* Put the ADC reading in the appropriate variable
    Current_state.ss.reading[channel] = (uint16_t)receive_UART0();
    Current_state.ss.reading[channel] = Current_state.ss.reading[channel] << 8;
    Current_state.ss.reading[channel] &= 0xFF00;
    Current_state.ss.reading[channel] |= (uint16_t)receive_UART0();
    channel++;
	
  }
  for (int i=0;i<6;i=i+1)
  {
	  Current_state.ss.read[i] = (double)(((double)Current_state.ss.reading[i])*3.3/1024.0);
  }
  /*uint8_t sen;
  for(int i=0;i<6;i=i+1)
  {
	  sen = (Current_state.ss.read[i]*255.0/3.3);
	  transmit_UART0(sen);
  }*/
  ///Power Down PD low
  //cbi(PORTB, PB6);
//return;  
}