/**
 * @file slave.c
 * @brief Slave main file
 */

#include "common.h"
#include "comm.h"
#include "spi.c"
#include "eeprom.h"
#include "slave_comm.h"
#include "peripherals.h"
#include "ax25.c"
#define F_CPU 8000000
#include <util/delay.h>
#include "uart.h"
#include "uart.c"
#include <avr/interrupt.h>
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 */

/* $Id: twitest.c,v 1.6 2005/11/05 22:32:46 joerg_wunsch Exp $ */

/*
 * Simple demo program that talks to a 24Cxx I²C EEPROM using the
 * builtin TWI interface of an ATmega device.
 */

/**
 * @file eeprom.c
 * @brief Interface with the EEPROM Device
 */

#include "common.h"
#include <util/twi.h>		/* Note [1] */


//#include "eeprom.h"

/**
 * Note [3]
 * TWI address for 24Cxx EEPROM:
 *
 * 1 0 1 0 E2 E1 E0 R/~W	24C01/24C02
 * 1 0 1 0 E2 E1 A8 R/~W	24C04
 * 1 0 1 0 E2 A9 A8 R/~W	24C08
 * 1 0 1 0 A10 A9 A8 R/~W	24C16
 */
static uint8_t eeprom_addr = 0b10100110;	/* E2 E1 E0 = 0 0 0 */

///Variables required for receiving data thru the SPI interface
volatile char message[19];
char message1[12]; //LatLongAlt extraction
char message2[7]; //HM extraction
volatile uint8_t end = 0, t = 0, process = 0;

static uint32_t read_addr = 0, write_addr = 0;
volatile unsigned int normal = 1;
volatile unsigned int downlink = 0; //inner circle
volatile unsigned int uplink = 0;
volatile unsigned int downlink_outer = 0; // outer circle
volatile unsigned int bytesToRead = 7;
volatile unsigned int JustDownlink = 0;
volatile unsigned int JustDownlinkOuter = 0;
/****************************/
volatile uint8_t temp; 
int Datasize = 7;
int Datasize2 = 11;
int max_addr = 131072;
uint8_t read_addr_dummy;
uint8_t write_addr_dummy;
int Last_Write = max_addr - (max_addr)%(Datasize2);
/**
 * @brief Interrupt service routine for SPI
 */

void ioinit(void)
{

  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
#if defined(TWPS0)
  /* has prescaler (mega128 & newer) */
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			/* smallest TWBR value, see note [5] */
#else
  TWBR = (F_CPU / 100000UL - 16) / 2;
#endif
}

int eeprom_read_bytes(uint32_t eeaddr, int len, uint8_t *buf)
{
  if((eeaddr < HALF_ADDR) && ((eeaddr + len) > HALF_ADDR))
  {
    int first = HALF_ADDR - eeaddr;
    eeprom_read_bytes_part(eeaddr, first, buf);
    return eeprom_read_bytes_part(HALF_ADDR, len - first , buf + first);
  }
  
  return eeprom_read_bytes_part(eeaddr, len, buf);
}

int eeprom_read_bytes_part(uint32_t eeaddr, int len, uint8_t *buf)
{
  uint8_t sla, twcr, n = 0;
  int rv = 0;
  
  ///* Added code for handling the two halves of the EEPROM
  if(eeaddr >= HALF_ADDR)
  {
    eeaddr -= HALF_ADDR;
    eeprom_addr |= 0x08;
  }
  else
  {
    eeprom_addr &= ~0x08;
  }
  
  /* patch high bits of EEPROM address into SLA */
  sla = eeprom_addr;

  /*
   * Note [8]
   * First cycle: master transmitter mode
   */
 restart:
  if (n++ >= MAX_ITER)
    return -1;
 begin:

  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_REP_START:		/* OK, but should not happen */
    case TW_START:
      break;

    case TW_MT_ARB_LOST:	/* Note [9] */
      goto begin;

    default:
      return -1;		/* error: not in start condition */
      /* NB: do /not/ send stop condition */
    }

  /* Note [10] */
  /* send SLA+W */
  TWDR = sla | TW_WRITE;
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_SLA_ACK:
      break;

    case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
      /* Note [11] */
      goto restart;

    case TW_MT_ARB_LOST:	/* re-arbitrate */
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }

  TWDR = (eeaddr>>8);		/* high 8 bits of addr */
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_DATA_ACK:
      break;

    case TW_MT_DATA_NACK:

    case TW_MT_ARB_LOST:
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }

  TWDR = eeaddr;		/* low 8 bits of addr */
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_DATA_ACK:
      break;

    case TW_MT_DATA_NACK:
      goto quit;

    case TW_MT_ARB_LOST:
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }

  /*
   * Note [12]
   * Next cycle(s): master receiver mode
   */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send (rep.) start condition */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_START:		/* OK, but should not happen */
    case TW_REP_START:
      break;

    case TW_MT_ARB_LOST:
      goto begin;

    default:
      goto error;
    }

  /* send SLA+R */
  TWDR = sla | TW_READ;
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MR_SLA_ACK:
      break;

    case TW_MR_SLA_NACK:
      goto quit;

    case TW_MR_ARB_LOST:
      goto begin;

    default:
      goto error;
    }

  for (twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);	len > 0;len--)
    {
      if (len == 1)
	twcr = _BV(TWINT) | _BV(TWEN); /* send NAK this time */
      TWCR = twcr;		/* clear int to start transmission */
      while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
      switch ((twst = TW_STATUS))
	{
	case TW_MR_DATA_NACK:
	  len = 0;		/* force end of loop */
				/* FALLTHROUGH */
	case TW_MR_DATA_ACK:
	  *buf++ = TWDR;
	  rv++;
	  break;

	default:
	  goto error;
	}
    }
 quit:
  /* Note [14] */
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */

  return rv;

 error:
  rv = -1;
  goto quit;
}

int eeprom_write_page(uint32_t eeaddr, int len, uint8_t *buf)
{
  uint8_t sla, n = 0;
  int rv = 0;
  uint16_t endaddr;
  
  ///* Added code for handling the two halves of the EEPROM
  if(eeaddr >= HALF_ADDR)
  {
    eeaddr -= HALF_ADDR;
    eeprom_addr |= 0x08;
  }
  else
    eeprom_addr &= ~0x08;

  if (eeaddr + len < (eeaddr | (PAGE_SIZE - 1)))
    endaddr = eeaddr + len;
  else
    endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;
  len = endaddr - eeaddr;

  /* patch high bits of EEPROM address into SLA */
  sla = eeprom_addr;

 restart:
  if (n++ >= MAX_ITER)
    return -1;
 begin:

  /* Note [15] */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_REP_START:		/* OK, but should not happen */
    case TW_START:
      break;
    case TW_MT_ARB_LOST:
      goto begin;
    default:
      return -1;		/* error: not in start condition */
      /* NB: do /not/ send stop condition */
    }

  /* send SLA+W */
  TWDR = sla | TW_WRITE;
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_SLA_ACK:
      break;

    case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
      goto restart;

    case TW_MT_ARB_LOST:	/* re-arbitrate */
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }
	
	
  TWDR = (eeaddr>>8);		/* high 8 bits of addr */
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_DATA_ACK:
      break;

    case TW_MT_DATA_NACK:
      goto quit;

    case TW_MT_ARB_LOST:
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }


  TWDR = eeaddr;		/* low 8 bits of addr */
  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
  switch ((twst = TW_STATUS))
    {
    case TW_MT_DATA_ACK:
      break;

    case TW_MT_DATA_NACK:
      goto quit;

    case TW_MT_ARB_LOST:
      goto begin;

    default:
      goto error;		/* must send stop condition */
    }

  for (; len > 0; len--)
    {
      TWDR = *buf++;
      TWCR = _BV(TWINT) | _BV(TWEN); /* start transmission */
      while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
      switch ((twst = TW_STATUS))
	{
	case TW_MT_DATA_NACK:
	  goto error;		/* device write protected -- Note [16] */

	case TW_MT_DATA_ACK:
	  rv++;
	  break;

	default:
	  goto error;
	}
    }
 quit:
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */

  return rv;

 error:
  rv = -1;
  goto quit;
}

int eeprom_write_bytes(uint32_t eeaddr, int len, uint8_t *buf)
{
  int rv, total;

  total = 0;
  do
    {
      rv = eeprom_write_page(eeaddr, len, buf);
      if (rv == -1)
        return -1;
      eeaddr += rv;
      len -= rv;
      buf += rv;
      total += rv;
    }
  while (len > 0);

  return total;
}

void error(void)
{

  exit(0);
}


uint8_t ADC_Convert( int channel)// ADC Initialization and Conversion combined in one function. Channel in switch case corresponds to the HM_Data number. See the assigned numbers on top
{
	switch(channel)
	{
		case 0: ADMUX = 0x60; // v1- Battery Voltage by 3
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 1:	ADMUX = 0x61;//v3 - OBC Voltage by 2
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 2: ADMUX = 0x62;//v2 - Downlink voltage by 2
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 3: ADMUX = 0x63;//i1 - Panel current
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
	}
}




void write_frame_to_eeprom(uint8_t *frame, int size)
{
  //correct the eeprom function for supporting 32-bit addresses
  static uint8_t start = 0;
  
  //if((write_addr == read_addr) && start)
    //read_addr += FRAME_SIZE;
    
  eeprom_write_bytes(write_addr, size,frame); //FRAME_SIZE replaced by 8
  write_addr += size;
  
  if(write_addr == Last_Write)
    write_addr = 0;
  
  start = 1;
}

void read_frame_from_eeprom(uint8_t *frame)
{
	if (JustDownlink == 2) //Position the read pointer when first Read command is called after Downlink turned ON
	{
		read_addr = write_addr;
		JustDownlink = 3;
		read_addr -= Datasize2; //Read pointer goes back 22 steps to read forward
	}
	
	eeprom_read_bytes(read_addr, Datasize2, frame); //FRAME_SIZE replaced by 40
	if(read_addr == 0)
	{
	read_addr = Last_Write;
	//read_addr -= Datasize2;
	}
	
	read_addr -= Datasize2;//Position the read pointer for the next read operation
	
	
	//if(read_addr + Datasize2 > MAX_ADDR)
	//read_addr = 0;
}

/** @brief Main functionP
 *  @todo Complete slave coding
 */

ISR(SPI_STC_vect)
{
	uint8_t dummy = SPDR;
	//transmit_UART0(dummy);
	message[t] = dummy;
	if(JustDownlink ==1){JustDownlink=2;}
	if(JustDownlinkOuter ==1){JustDownlinkOuter=2;}	
	if((message[0] == 0xF1)&&(message[1] == 0xF1)&&(message[2] == 0xF1))
	{
		normal = 0;
		downlink = 1;
		uplink = 0;
		downlink_outer = 0;
		bytesToRead = 19;
		JustDownlink = 1;
		JustDownlinkOuter = 0;
		for(int i =0; i<19; i++)
		{
			message[i] = 0;
		}
		t = 0;
	}
	else if((message[0] == 0xB1)&&(message[1] == 0xB1)&&(message[2] == 0xB1))
	{
		normal = 0;
		downlink = 0;
		downlink_outer = 1;
		uplink = 0;
		JustDownlink=0;
		JustDownlinkOuter = 1;
		bytesToRead = 19;
		//UCSR1B&= ~(1<<TXEN1);
		for(int i =0; i<19; i++)
		{
			message[i] = 0;
		}
		t = 0;
		
	}
	else if((message[0] == 0xD1)&&(message[1] == 0xD1)&&(message[2] == 0xD1))
	{
		normal = 1;
		downlink = 0;
		uplink = 0;
		downlink_outer = 0;
		bytesToRead = 7;
		//UCSR1B&= ~(1<<TXEN1);
		for(int i =0; i<19; i++)
		{
			message[i] = 0;
		}
		t = 0;
		
	}
	else if( t>=(bytesToRead-1)) {
		//message[t] = dummy;
			t=0;
			end = 1;
			 //transmit_UART0('}');
		}else {
			t++;
		}
/*
	if(!process)
	{
		message[t] = dummy;
		transmit_UART0(message[t]);
		//_delay_ms(100);
		if(message[t] == 0xAA){
			end++;
			} else if(end != 0) {
			end = 0;
		}
		if(end == 2){
			process = 1;
		}
		t++;
	}
	*/
}




int main(void) {
	uint8_t transmission = 0, command;
	uint16_t crc, recv_crc;
	char* data;
	DDRA=0x0F;
	PORTA=0x0F;
	DDRF = 0x00;
	 uint8_t frame[256];
	init_UART0();
	init_UART1();
	//transmit_string_UART0("ini");
	//inituart();
	transmit_string_UART0("Slave");
	///Disable Watchdog Timer
	wdt_disable();
	///Enable Global Interrupts
	init_SPI_slave();
	//ioinit();
	sei();
	///Initialise SPI as slave
	
	char rx_eeprom[Datasize2];
	char rx_eeprom2[Datasize2];
	char rx[34];
	uint8_t ADC_Result;
	uint8_t Temperature[4];
	PORTA=0x0A;
	//int counter = 1;		
	while(1)
	{
			UCSR1B &= ~(1<<TXEN1);//UART1 is for downlink only. Enabling it unneccasarily may lead to unexpected powering of Downlink
			UCSR0B |= _BV(TXEN0);
			//transmit_UART0('t');
			ADC_Result = ADC_Convert(0);
			if(end == 1){transmit_UART0('a');}
			Temperature[0] = ADC_Result;//*3.3/256; Verify this
			if(end == 1){transmit_UART0('b');}
			if(end == 1){transmit_UART0(Temperature[0]);}//USARTWriteChar(HM_Data[i]);
			_delay_us(100);
		
		    ADC_Result = ADC_Convert(1);
			if(end == 1){transmit_UART0('c');}
			Temperature[1] = ADC_Result;//*3.3/256; Verify this
			if(end == 1){transmit_UART0('d');}
			if(end == 1){transmit_UART0(Temperature[1]);}//USARTWriteChar(HM_Data[i]);
			_delay_us(100);
			
			ADC_Result = ADC_Convert(2);
			if(end == 1){transmit_UART0('e');}
			Temperature[2] = ADC_Result;//*3.3/256; Verify this
			if(end == 1){transmit_UART0('f');}
			if(end == 1){transmit_UART0(Temperature[2]);}//USARTWriteChar(HM_Data[i]);
			_delay_us(100);
			
			ADC_Result = ADC_Convert(3);
			if(end == 1){transmit_UART0('g');}
			Temperature[3] = ADC_Result;//*3.3/256; Verify this
			if(end == 1){transmit_UART0('h');}
			if(end == 1){transmit_UART0(Temperature[3]);}//USARTWriteChar(HM_Data[i]);
			_delay_us(100);
			UCSR0B&= ~(1<<TXEN0);
			
		if(normal==1) //Normal mode
		{	
		if (end == 1) {
			//transmit_UART0('k');
			//transmit_UART0('l');
			//transmit_UART0('m');
			//transmit_string_UART0("normal");	
		//transmit_string_UART0((char *)message);
		_delay_ms(400); 
		cli();	
		write_frame_to_eeprom((uint8_t*)message, 7);
		sei();
		write_frame_to_eeprom((uint8_t*)Temperature, 4);
		_delay_ms(500);
		end = 0;
		}
		}
		else if(downlink == 1)
		{
		//_delay_ms(50);
		while(JustDownlink ==1);	
		if(end==1){
			cli();
			for(int k = 0; k<7; k++){message2[k] = message[k];}
			for(int k = 7; k<19; k++){message1[k-7] = message[k];}
				sei();
			write_frame_to_eeprom((uint8_t*)message2, 7);
			write_frame_to_eeprom((uint8_t*)Temperature, 4);
				end =0;}
			//transmit_string_UART0("Downlink");
		read_frame_from_eeprom((uint8_t*)rx_eeprom);
		read_frame_from_eeprom((uint8_t*)rx_eeprom2);
		//read_frame_from_eeprom((uint8_t*)rx2);
		//rx[8]='\0';
		//_delay_ms(300);
		//transmit_string_UART0((char*)rx);
		for(int j = 0; j<Datasize2; j++)
		{
			rx[j] = rx_eeprom[j];
		}
		
		for(int j = Datasize2; j<2*Datasize2; j++)
		{
			rx[j] = rx_eeprom2[j-Datasize2];
		}
		
		for(int j= 2*Datasize2; j<34; j++)
		{
			rx[j] = message1[j-2*Datasize2];
		}
		
		make_ax25_frame_from_data(frame, (uint8_t *)rx);
		
		
		UCSR1B |=(1<<TXEN1);
		_delay_ms(10);
		for(int i =0; i<(27+2*Datasize2+12); i= i+1)
		{
			transmit_UART1(frame[i]);	
		}
		UCSR1B&= ~(1<<TXEN1);
		
		read_addr_dummy = (uint8_t)read_addr;//Remove from final
		read_addr_dummy = (uint8_t)write_addr;//Remove from final
		
		transmit_UART0(read_addr_dummy);//Remove from final
		transmit_UART0(write_addr_dummy);//Remove from final
		
		
		
		_delay_ms(200);
		}
		//else if(uplink == 1){_delay_ms(900);}
		
		
		else if(downlink_outer == 1)
		{
			while(JustDownlinkOuter ==1);
			if(end==1){
				cli();
				for(int k = 0; k<7; k++){message2[k] = message[k];}
				for(int k = 7; k<19; k++){message1[k-7] = message[k];}
					sei();
				write_frame_to_eeprom((uint8_t*)message2, 7);
				write_frame_to_eeprom((uint8_t*)Temperature, 4);
			end =0;}
			
			//read_frame_from_eeprom((uint8_t*)rx_eeprom);

			
			for(int j= 0; j<12; j++)
			{
				rx[j] = message1[j];
			}
			
			for (int j = 12; j<24; j++)
			{
				rx[j] = message1[j-12];
			}
			
			for(int j = 24; j< 33; j++)
			{
			rx[j] = 0;
			}			
			
			make_ax25_frame_from_data(frame, (uint8_t *)rx);
			
			
			UCSR1B |=(1<<TXEN1);
			_delay_ms(10);
			for(int i =0; i<(27+2*Datasize2+12); i= i+1)
			{
				transmit_UART1(frame[i]);
			}
			UCSR1B&= ~(1<<TXEN1);
			read_addr_dummy = (uint8_t)read_addr;//Remove from final
			read_addr_dummy = (uint8_t)write_addr;//Remove from final
			
			transmit_UART0(read_addr_dummy);//Remove from final
			transmit_UART0(write_addr_dummy);//Remove from final
			
			_delay_ms(200);
		}
			
		/*_delay_ms(300);
		uint16_t i = n         0;
  	///Loop through the data
  	while(rx[i]!='\0')
  	{
    	///* Wait for empty transmit buffer 
    	while ( !(UCSR0A & (_BV(UDRE0))) );
    	///* Start transmission
    	UDR0 = rx[i];
		i++;	
	}*/
			//transmit_string_UART0("good");	
	//	}*/
		}
		
	 return 0;
	}
		
		/*
		if (process)
		{		transmit_string_UART0("good");
			//crc = calculate_crc_16((uint8_t *)message, t - (end + 1));
			//memcpy((void *)&recv_crc, (void *)&(message[t - (end + 1)]), sizeof(uint16_t));
			//write_frame_to_eeprom((uint8_t*)message);
	
			//			transmit_string_UART0("t =");
				//		transmit_UART0(t);
						//transmit_string_UART0("Message=");
						//transmit_string_UART0((char *)message);

	

		//	read_frame_from_eeprom((uint8_t*)rx);
			//rx[3]='\0';
			//transmit_string_UART0((char*)rx);
			PORTA=0xFF;
	

			//char crc_lsb = crc&&0x00ff;
			//char crc_msb = crc>>8;
			//transmit_string_UART0((char *)crc);
			//transmit_UART0(crc_lsb);
			//transmit_UART0(crc_msb);
			t=0;
			process=0;
			end=0;
			
		}
	}*/

 







