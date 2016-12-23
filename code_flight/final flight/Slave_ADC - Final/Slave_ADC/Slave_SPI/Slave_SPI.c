/*
 * slave_adc.cpp
 *
 * Created: 12/13/2014 2:54:16 PM
 *  Author: itachi & Yash Sanghavi
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//uint8_t data_t;
 volatile uint8_t data_r;
 volatile uint8_t data_r1;
 volatile int counter = 0;
 volatile int  SetComplete = 1;

uint8_t valh1;
uint8_t vall1;
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

void SPI_SlaveInit(void)
{
	DDRB=0x10;
	//Set MISO output and all others input
	//EnableSPI, Master set clock rate fosc/128, enable interrupt, DORD zero hence msb transmitted first
	SPCR = 0x00;
	SPCR|=(1<<SPE)|(1<<SPIE)|(1<<CPOL)|(1<<CPHA);
}

uint8_t SPI_Slavetransceive(uint8_t data_t)
{
	SPDR=data_t;
	//Wait for reception complete
	while(!(SPSR & (1<<SPIF)));
	data_r=SPDR;
	
	return data_r; //return data
}

void SlaveReceive(void)
{
	while(!(SPSR & (1<<SPIF)));
}


void init_adc(void)
{
	DDRC=0x00; //ADC pins are inputs
	ADCSRA=0x86; //0b10000110. Enable ADC. Value of prescaler is 64.
	ACSR=0x80; //Disable analog comparator
}
uint16_t adc(uint8_t mux_input)
{
	uint16_t output1;
	uint16_t output2;
	mux_input=mux_input & 0x07;                         //0b00000111;
	//ADMUX = 0x20| mux_input;             //left adjust result
	ADMUX = mux_input;                   //right adjust result
	ADCSRA |= (1<<ADSC);                                // start adc conversion
	
	while(((ADCSRA & 0x10)==0));
	output1=ADCL;
	output2=ADCH;
	output2=(output2<<8)+output1;
	ADCSRA = ADCSRA|0x10;  //clear ADCIF (ADC Interrupt Flag) by writing 1 to it
	
	return output2;
}

uint8_t adc_hvalue(uint16_t output2)
{
uint8_t hvalue=(output2>>8);	
return hvalue;        
                
}
uint8_t adc_lvalue(uint16_t output2)
{
uint8_t lvalue=output2;              
return lvalue;			    
}

ISR(SPI_STC_vect)
{
	data_r1 = SPDR;
	counter = counter+1;
	SetComplete = 0;
	switch(counter)
	{
		case 1: SPDR = vall1;
		break;
		case 2: SPDR = valh1;
		break;
		case 3: SPDR = vall2;
		break;
		case 4: SPDR = valh2;
		break;
		case 5: SPDR = vall3;
		break;
		case 6: SPDR = valh3;
		break;
		case 7: SPDR = vall4;
		break;
		case 8: SPDR = valh4;
		break;
		case 9: SPDR = vall5;
		break;
		case 10: SPDR = valh5;
		break;
		case 11: SPDR = vall6;
		break;
		case 12: SPDR = valh6;
		//counter = 0;
		break;
		case 13: counter = 0;
		SetComplete = 1;
	}
}

int main(void)
{
	
	DDRD=0xFF;

	//_delay_ms(2000);
	//PORTA IS FOR ADC CHANNELS IN ATMEGA32
	uint16_t output0;
	uint16_t output1;
	uint16_t output2;
	uint16_t output3;
	uint16_t output4;
	uint16_t output5;
	
	SPI_SlaveInit();
	init_adc();
	sei();
	//DDRC = 0x00;
	
	//DDRC = 0xFF;	
	
	while(1)
	{
	
	//for 10 bit conversion read ADCL first	
	
	if (SetComplete == 1)
	{
	output0=adc(0x00);	
	vall1=adc_lvalue(output0);
	valh1=adc_hvalue(output0);

	output1=adc(0x01);
	vall2=adc_lvalue(output1);
	valh2=adc_hvalue(output1);
	
	output2=adc(0x02);
	vall3=adc_lvalue(output2);
	valh3=adc_hvalue(output2);
	
	output3=adc(0x03);
	vall4=adc_lvalue(output3);
	valh4=adc_hvalue(output3);
	
	output4=adc(0x04);
	vall5=adc_lvalue(output4);
	valh5=adc_hvalue(output4);
	
	output5=adc(0x05);
	vall6=adc_lvalue(output5);
	valh6=adc_hvalue(output5);		
	PORTD=valh2;
	SetComplete = 0;
	}
	//data_r = SPI_Slavetransceive(0xF1);
	
	//PORTC=data_r1;
	}
}
