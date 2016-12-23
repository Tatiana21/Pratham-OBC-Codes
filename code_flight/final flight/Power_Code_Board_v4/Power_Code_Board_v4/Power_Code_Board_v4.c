/*
 * Power_Code_Board_v4.c
 *
 * Created: 01-Mar-15 10:15:27 PM
 *  Author: YSanghvi
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/wdt.h>
#include "uart_8_32.c"

unsigned char write_data,recv_data;
volatile uint8_t killFlag = 0;
uint8_t OC2,OC3,OC4,OC5,OC6;
unsigned int WasUnderVoltage = 0;
uint8_t BatteryVoltage;
volatile int i2c_after_reset = 0;
int PowerSaving = 0;
int Safe = 0;
int beacon = 0;
int KillCounter = 0;

volatile uint8_t HM_Data[7],j=0;
#define Loadstatus HM_Data[0];// These semicolons not there in PIC code. Verify
#define BatteryVoltageBy3 HM_Data[1];
#define DownlinkVoltageBy2 HM_Data[2];
#define OBCVoltageBy2 HM_Data[3];
#define TotalCurrent HM_Data[4];
#define ConsumptionCurrent HM_Data[5];



#define PBEACON     7
#define PCONTROL    6
#define PGPS        5
#define PDOWNLINK   4
#define POBC        3
#define PMAG        2
#define Uplink		1

volatile unsigned int CommandByte;

void Startup_all() // Start all the loads
{
	PORTA = 0x00;
	PORTB = 0x0D;
	PORTC = 0xC4;
	PORTD = 0x10;
	HM_Data[0] = 0xFE; // In the PIC code, it is EC. Please verify
}

void Startup() // Turn on Beacon, Control, OBC
{
	PORTA = 0x40;
	PORTD = 0x01;
	PORTC = 0x04;
	PORTB = 0x10;
	HM_Data[0] = 0xC8;
}

void Shutdown() // Turn off all loads
{
	PORTA = 0xC0;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0xC0;
	HM_Data[0] = 0x00;
}

void PowerSavingMode(void) //Keep only Beacon On. To be entered when Vbat<6.6V 
{
	if(beacon == 1)
	{
	PORTA = 0xC0;
	PORTB = 0x01;
	PORTC = 0x00;
	PORTD = 0x80;
	HM_Data[0] = 0x80;
	}
	else{
		Shutdown();
	HM_Data[0] = 0x00;
	}
}



void SafeMode(void) //To be entered when Vbat<6V
{
	Shutdown();
	return;
}

void Kill()// When kill command received from the Uplink
{
	Shutdown();
	while(1);
}

void Sat_reset()//When reset command received from the Uplink
{
	Shutdown();
	return;
}

void Sleep()
{
	unsigned int count = 0;
	while(count!= 3000)										// 50 minutes delay after the snap
	{
		//	__delay_ms(1000);
		count = count + 1;
	}
}

void OBCcommandResponse (void) // Switch loads on and off depending on Command of OBC
{
	if((CommandByte & 0x80) == 0x80) //Beacon
	{
		PORTB |= (1<<PB0);  // Enable TPS of Beacon
		PORTD &= ~(1<<PD6); // Disable Inhibit PTH of Beacon
		beacon = 1;
	}
	else
	{
		PORTB &= ~(1<<PB0);  // Disable TPS of Beacon
		PORTD |= (1<<PD6);  // Inhibit PTH of Beacon
		beacon = 0;
	}
	
	if((CommandByte & 0x40) == 0x40) //Control
	{
		PORTD |= (1<<PD4);  //Enable TPS of Control
		PORTD &= ~(1<<PD7); // Turn off Inhibit of PTH of Control
	}
	else
	{
		PORTD &= ~(1<<PD4); // Disable TPS of Control
		PORTD |= (1<<PD7);// Inhibit PTH of Control
	}
	
	if((CommandByte & 0x20) == 0x20) //GPS
	{
		PORTC |= (1<<PC7); //Enable TPS of GPS
		PORTA &= ~(1<<PA6);// Turn off Inhibit of PTH of GPS
	}
	else
	{
		PORTC &= ~(1<<PC7);// Disable TPS of GPS
		PORTA |= (1<<PA6);// Inhibit PTH of GPS
	}
	
	
	if((CommandByte & 0x08) == 0x08) //OBC
	{
		PORTC |= (1<<PC2);// Enable TPS of OBC
		PORTA &= ~(1<<PA7);// Turn off Inhibit of PTH of OBC
	}
	else
	{
		PORTC &= ~(1<<PC2);// Disable TPS of OBC
		PORTA |= (1<<PA7);// Inhibit PTH of OBC
	}
	
	if((CommandByte & 0x04) == 0x04) //MAG
	{
		PORTB |= (1<<PB3);// Enable switch of MAG
	}
	else
	{
		PORTB &= ~(1<<PB3);// Disable switch of MAG
	}
	
	if((HM_Data[0] & 0x10) == 0x10)
	{
		if((CommandByte & 0x10) == 0x10) //Downlink
		{
			PORTB |= (1<<PB2);// Enable TPS of Downlink
			PORTD &= ~(1<<PD6);// Turn off Inhibit of PTH of Downlink
		}
		else
		{
			PORTB &= ~(1<<PB2);// Disable TPS of Downlink
			//PORTD |= (1<<PD7);// Can't inhibit PTH as beacon connected to same PTH
		}
	
	
		if ((CommandByte & 0x02) == 0x02)
		{
			PORTC |= (1<<PC6); //Enable Uplink
		}
		else
		{
			PORTC &= ~(1<<PC6);//Disable Uplink
		}
	}
	else
	{
		if ((CommandByte & 0x02) == 0x02)
		{
			PORTC |= (1<<PC6); //Enable Uplink
		}
		else
		{
			PORTC &= ~(1<<PC6);//Disable Uplink
		}
		
		if((CommandByte & 0x10) == 0x10) //Downlink
		{
			PORTB |= (1<<PB2);// Enable TPS of Downlink
			PORTD &= ~(1<<PD6);// Turn off Inhibit of PTH of Downlink
		}
		else
		{
			PORTB &= ~(1<<PB2);// Disable TPS of Downlink
			//PORTD |= (1<<PD7);// Can't inhibit PTH as beacon connected to same PTH
		}
	}
	
	
	
	
	
	HM_Data[0] = CommandByte; 
}

uint8_t ADC_Convert( int channel)// ADC Initialization and Conversion combined in one function. Channel in switch case corresponds to the HM_Data number. See the assigned numbers on top
{
	switch(channel)
	{
		case 1: ADMUX = 0x61; // v1- Battery Voltage by 3
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 2:	ADMUX = 0x63;//v3 - OBC Voltage by 2
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 3: ADMUX = 0x64;//v2 - Downlink voltage by 2
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 4: ADMUX = 0x60;//i1 - Panel current
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
		
		case 5: ADMUX = 0x62;//i2 - Consumption Current
		ADCSRA = 0xC5;
		_delay_ms(10);
		while (ADCSRA & (1<<ADSC));
		return ADCH;
		break;
	}
}

void TWI_init_slave(void) // Function to initilaize slave
{
//TWCR=0x01;
TWAR=0x20; // Fill slave address to TWAR
}
 
void TWI_write_slave(void) // Function to write data
{
TWDR= write_data;          // Fill TWDR register whith the data to be sent 
TWCR= (1<<TWEN)|(1<<TWINT);   // Enable TWI, Clear TWI interrupt flag 
while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}
 
void TWI_match_write_slave(void) //Function to match the slave address and slave dirction bit(write) 
{
while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknoledgement have been received
{
// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
}
}
 
void TWI_read_slave(void)
{
// Clear TWI interrupt flag,Get acknowlegement, Enable TWI
TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
recv_data=TWDR; // Get value from TWDR
//PORTB=recv_data; // send the receive value on PORTB
}

void TWI_match_read_slave(void) //Function to match the slave address and slave dirction bit(read)
{
/*while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
{
 
// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
PORTA=0xff;
}*/
TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
     //PORTB =0xf0;   // Fill slave address to TWAR
     while (!(TWCR & (1<<TWINT)));
	  
	 while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
    {
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
		//PORTA=0xff;
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));
		//PORTB = 0x0f; // Wait for TWINT flag
		//PORTB = 0xff;
    }
}

void StartOBC()
{
	PORTC |= (1<<PC2);
	PORTA &= ~(1<<PA7);
}

void StartDown()
{
	PORTB |= (1<<PB2);// Enable TPS of Downlink
	PORTD &= ~(1<<PD6);// Turn off Inhibit of PTH of Downlink
}

ISR(TWI_vect)
{
	//cli();
	//PORTB=0x0f;
	switch (TWSR & 0xF8)
	{
		case TW_SR_SLA_ACK:                 // 0x60: own SLA+W has been received, ACK has been returned
		case TW_SR_ARB_LOST_SLA_ACK:        // 0x68: own SLA+W has been received, ACK has been returned
		case TW_SR_GCALL_ACK:               // 0x70:     GCA+W has been received, ACK has been returned
		case TW_SR_ARB_LOST_GCALL_ACK:      // 0x78:     GCA+W has been received, ACK has been returned
		// we are being addressed as slave for writing (data will be received from master)
		// set state
		// receive data byte and return ACK
		TWCR|=(1<<TWINT)|(1<<TWEA);//PORTB=0xf0;
		//outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
			break;
		case TW_ST_SLA_ACK:
			                   TWDR= HM_Data[j];
							   TWCR|= (1<<TWEN)|(1<<TWINT)|(1<<TWEA);
			                   j=j+1;          // Fill TWDR register whith the data to be sent
			                   if(j==7){
								   j=0;
								   if(i2c_after_reset>=1)
								   {
									   i2c_after_reset = i2c_after_reset+1;		   }
								   break;}
			                   
			/*j=j+1;          // Fill TWDR register whith the data to be sent
			if(j==6)j=0;
			TWCR|= (1<<TWEN)|(1<<TWINT)|(1<<TWEA);*/
			break;
		case TW_ST_DATA_ACK:
		case TW_ST_DATA_NACK:  
		break;
		case TW_SR_DATA_ACK:                // 0x80: data byte has been received, ACK has been returned
		case TW_SR_GCALL_DATA_ACK:          // 0x90: data byte has been received, ACK has been returned
		// get previously received data byte
		CommandByte=TWDR;
		//write_data=~(recv_data);
		OBCcommandResponse();
		j = 0;
		TWCR|=(1<<TWINT)|(1<<TWEA);
		break;
		case TW_SR_STOP:
        TWCR|= (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);    //sei(); 
		break;
			
	}
	TWCR|= (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	//sei();
	//return 0;
} 

ISR(INT0_vect)
{
	GICR&=~(1<<INT0);
	
	unsigned int count = 0;
	while(count!=10)   //sleep for another 5min
	{
		_delay_ms(1000);
		count = count+1;
	}
}

ISR(USART_RXC_vect) {
	killFlag = isKillCodeRxd();
	//PORTC &= ~(1<<PC2);
	//PORTA |= (1<<PA7);
}

void main(void)
{
	DDRA = 0xC0; // I have set the PF Check as Input. Verify!!
	DDRB |= (1<<PB0)|(1<<PB2)|(1<<PB3); //Verify this way of assigning I/O to pins
	DDRC |= (1<<PC7)|(1<<PC2)|(1<<PC6);
	DDRD |= (1<<PD7)|(1<<PD6)|(1<<PD4);
	unsigned int ADC_Result;
	
	init_UART();
	UBRRL = 103;
	UCSRB |= _BV(RXCIE);
	
	TWI_init_slave(); // Function to initilaize slave
	TWCR|= (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	Shutdown();
	GICR = (1<<INT0);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	MCUCR = 0xA0;//Power down mode of sleep
	
	sei();
	
	sleep_enable();
	sleep_mode();
	sleep_disable();
	
	_delay_ms(100);    //do not remove this delay. Atmega wakes up and keeps executing before going to interrupt for a finite amount of time.
	GICR&=~(1<<INT0);  //precaution so that another snap interrupt may not occur
	//Startup_all();
	StartOBC();
	//StartDown();
	_delay_ms(1000);
	while(1)
	{
		if (killFlag == 1) {//Kill Condition
			KillCounter = KillCounter+1;
			if(KillCounter == 3)
			{cli();
			Shutdown();
			while(1);}
			killFlag = 0;
			} else if (killFlag == 2) {//Reset Condition
				for(int i =0; i<6; i++)
				{
					HM_Data[i] = 0x00;
				}
				i2c_after_reset = 1;
				while(i2c_after_reset != 7);
				_delay_ms(1000);
				cli();
			Shutdown();
			i2c_after_reset = 0;
			_delay_ms(5000);
			StartOBC();
			_delay_ms(5000);
			killFlag = 0;
			sei();

		}
		
	for(unsigned int i = 1; i<6;i=i+1)
		{
			ADC_Result = ADC_Convert(i);
			HM_Data[i] = ADC_Result;//*3.3/256; Verify this
			//USARTWriteChar(HM_Data[i]);
		}
		
		OC5 = PIND&(1<<PD3); //OC of Downlink
		OC4 = PINB&(1<<PB1);//OC of Beacon
		OC6 = PIND&(1<<PD5);//OC of control/torque
		OC3 = PINC&(1<<PC3); //OC of OBC
		OC2 = PINC&(1<<PC4);//OC of GPS
		
		HM_Data[6] = (OC4<<6)|(OC6<<1)|(OC2<<1)|(OC5<<1)|OC3;
		
		if((HM_Data[6]&(0x08))==0)
		{
			PORTC &= ~(1<<PC2);// Disable TPS of OBC
			PORTA |= (1<<PA7);// Inhibit PTH of OBC
			
		}
		else
		{
			PORTC |= (1<<PC2);// Enable TPS of OBC
			PORTA &= ~(1<<PA7);// Turn off Inhibit of PTH of OBC
		}
	
/*	
do											// Want to run this loop at least once. It will keep on checking if condition is true
{
	BatteryVoltage = ADC_Convert(1);
	HM_Data[1] = BatteryVoltage;
	
	if ((HM_Data[1] < 0xA2) & (HM_Data[1] > 0x93))
	{	PowerSavingMode();					// actually safe mode
		WasUnderVoltage = 1;
	}
	else if ((HM_Data[1] < 0x93))
	{	SafeMode();							// actually emergency mode
		WasUnderVoltage = 1;
	}
}while(HM_Data[1] < 0xA2);
*/


//*********************************Battery UnderVoltage Protection ***********************************************//
do{
	BatteryVoltage = ADC_Convert(1);
	HM_Data[1] = BatteryVoltage;
if((HM_Data[1] > 0xA2)&&(PowerSaving == 0)&&(Safe == 0)) ///>6.6V
{
	PowerSaving = 0;
	Safe = 0;
}

if((HM_Data[1] < 0xA2) && (HM_Data[1] > 0x93) && (PowerSaving == 0) && (Safe == 0))  //6 and 6.6V
{
PowerSaving = 1;
Safe = 0;
PowerSavingMode();
}

if(HM_Data[1] < 0x93) //6V
{
	PowerSaving = 1;
	Safe = 1;
	SafeMode();
}


if(Safe == 1)
{
if(HM_Data[1] > 0xAC)//7V
{
Safe = 0;
PowerSavingMode();
}
}

if(PowerSaving == 1)
{
	if(HM_Data[1] > 0xC6)//8v
	{
		PowerSaving = 0;
		StartOBC();
	}
} }while((PowerSaving == 1)||(Safe == 1));
//***********************************End of Battery Undervoltage Protection ********************************************//
/*
if(WasUnderVoltage == 1)
{
	StartOBC();
	WasUnderVoltage = 0;
}
*/
}
return;
	}

