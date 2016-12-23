/*
 * Trnasmit_cc.c
 *
 * Created: 02-10-2014 03:22:44
 *  Author: Someone Stud
 */ 
#define F_CPU 8000000
#include "uart.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>
#include <string.h>
#include <avr/interrupt.h>
//#include <uart.c>
//Global Variable
unsigned char data_transmit[61],data_receive[60] ,transmit_enable=0,receive_enable=1,SWITCH[8]="$SWITCH",switch_num=0,transmit_check=0;
unsigned char address[61]="SATELLITE_PRATHAM";
unsigned int pkt_length=48,check=0;
int transmitFlag = 0;

//UART baudrate calculation
//#define F_CPU 8000000			// oscillator-frequency in Hz, also needs delay.h header (Defined in Project->Configuration options)
#define UART_BAUD_RATE 9600
//for Asynchronous Normal mode (U2x=0) formula will be following:
#define UART_BAUD_CALC (((F_CPU/(UART_BAUD_RATE*16UL)))-1) //16UL still remains regardless of the frequency of crystal

// Atmega8

#define SPICS	2	//4	// Port B bit 2 (pin14): SS->chip select for CC
#define SPIDO	3	//5	// Port B bit 3 (pin15): MOSI->data out (data to CC_SI)
#define SPIDI	4	//6	// Port B bit 4 (pin16): MISO->data in (data from CC_SO)
#define SPICLK	5	//7	// Port B bit 5 (pin17): SCK->clock for CC

// CC1101 
#define CC_GDO0	0	// Port B bit 0 (pin12) (pin6 of cc)
#define CC_GDO2 1   // Port B bit 1 (pin13) (pin3 of cc)
#define CC_CSN 	2	//4   // (pin7 of cc):-> chip select
#define CC_SI 	3	//5	// (pin20 of cc):-> data input
#define CC_SO 	4	//6   // (pin2 of cc)-multiplexed with GDO1: -> data output
#define CC_SCLK 5	//7   // (pin1 of cc):-> clock input

// CC2500/CC1100/CC1101 STROBE, CONTROL AND STATUS REGISTER
#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high byte
#define CCxxx0_SYNC0        0x05        // Sync word, low byte
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high byte
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle byte
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low byte
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High byte Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test
#define CCxxx0_TEST2        0x2C        // Various test settings
#define CCxxx0_TEST1        0x2D        // Various test settings
#define CCxxx0_TEST0        0x2E        // Various test settings

// Strobe commands
#define CCxxx0_SRES         0x30        // Reset chip.
#define CCxxx0_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
#define CCxxx0_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
#define CCxxx0_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
#define CCxxx0_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
#define CCxxx0_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
#define CCxxx0_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CCxxx0_SPWD         0x39        // Enter power down mode when CSn goes high.
#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
#define CCxxx0_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                        // bytes for simpler software.
// Status registers (read & burst)
#define CCxxx0_PARTNUM      (0x30 | 0xc0)
#define CCxxx0_VERSION      (0x31 | 0xc0)
#define CCxxx0_FREQEST      (0x32 | 0xc0)
#define CCxxx0_LQI          (0x33 | 0xc0)
#define CCxxx0_RSSI         (0x34 | 0xc0)
#define CCxxx0_MARCSTATE    (0x35 | 0xc0)
#define CCxxx0_WORTIME1     (0x36 | 0xc0)
#define CCxxx0_WORTIME0     (0x37 | 0xc0)
#define CCxxx0_PKTSTATUS    (0x38 | 0xc0)
#define CCxxx0_VCO_VC_DAC   (0x39 | 0xc0)
#define CCxxx0_TXBYTES      (0x3A | 0xc0)
#define CCxxx0_RXBYTES      (0x3B | 0xc0)

#define CCxxx0_PATABLE      0x3E
#define CCxxx0_TXFIFO       0x3F
#define CCxxx0_RXFIFO       0x3F

//-------------------------------------------------------------------------------------------------------
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
typedef struct RF_SETTINGS {
    // Rf settings for CC1101
	  unsigned char  IOCFG0;          //GDO0 Output Pin Configuration
	  unsigned char  FIFOTHR;         //RX FIFO and TX FIFO Thresholds
	  unsigned char  PKTCTRL0;		  //Packet Automation Control
	  unsigned char  FSCTRL1;         //Frequency Synthesizer Control
	  unsigned char  FREQ2;           //Frequency Control Word, High Byte
	  unsigned char  FREQ1;           //Frequency Control Word, Middle Byte
	  unsigned char  FREQ0;           //Frequency Control Word, Low Byte
	  unsigned char  MDMCFG4;         //Modem Configuration
	  unsigned char  MDMCFG3;         //Modem Configuration
	  unsigned char  MDMCFG2;         //Modem Configuration
	  unsigned char  DEVIATN;         //Modem Deviation Setting
	  unsigned char  MCSM0;           //Main Radio Control State Machine Configuration
	  unsigned char  FOCCFG;          //Frequency Offset Compensation Configuration
	  unsigned char  WORCTRL;         //Wake On Radio Control
	  unsigned char  FSCAL3;          //Frequency Synthesizer Calibration
	  unsigned char  FSCAL2;          //Frequency Synthesizer Calibration
	  unsigned char  FSCAL1;          //Frequency Synthesizer Calibration
	  unsigned char  FSCAL0;          //Frequency Synthesizer Calibration
	  unsigned char  TEST2;           //Various Test Settings
	  unsigned char  TEST1;           //Various Test Settings
	  unsigned char  TEST0;           //Various Test Settings
} RF_SETTINGS;

unsigned char ccxxx0_Strobe(unsigned char);
unsigned char ccxxx0_Read(unsigned char);
unsigned char ccxxx0_Write(unsigned char, unsigned char);
void ccxxx0_ReadBurst(unsigned char, unsigned char*, unsigned int);
void ccxxx0_WriteBurst(unsigned char, unsigned char*, unsigned int);

void ccxxx0_PowerOnReset();

void ccxxx0_Setup(const RF_SETTINGS*);

void ATMEGA_Init(void);
void init_UART0(void);
void transmit_UART0(char);
void transmit_string_UART0(char *);
uint8_t receive_UART0();

void CC_Transmit();
void  CC_Receive();

void ATMEGA_Init(void) 
{	
	DDRC =0x01;
	PORTC=0x01;

	init_UART0();

// SPI register config
	DDRB &= ~(1 << SPIDI);	// set port B SPI data input to input
	DDRB |= (1 << SPICLK) ;	// set port B SPI clock to output
	DDRB |= (1 << SPIDO);	// set port B SPI data out to output 
	DDRB |= (1 << SPICS);	// set port B SPI chip select to output
	DDRB &= ~(1 << CC_GDO0);	// set port B packet received pin to input
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0) ;//| (1 << SPI2X) ;// | (1 << SPR1) ;// | (1 << SPR0);
	SPSR = 0x00;	

	PORTB |= (1 << SPICS);	// set chip select to high (CC is NOT selected)
	PORTB &= ~(1 << SPIDO);	// data out =0
	PORTB |= (1 << SPICLK); // clock out =1
	
}


/* Deviation = 197.753906 */
/* Base frequency = 433.999786 */
/* Carrier frequency = 433.999786 */
/* Channel number = 0 */
/* Carrier frequency = 433.999786 */
/* Modulated = true */
/* Modulation format = GFSK */
/* Manchester enable = false */
/* Sync word qualifier mode = 30/32 sync word bits detected */
/* Preamble count = 4 */
/* Channel spacing = 199.813843 */
/* Carrier frequency = 433.999786 */
/* Data rate = 2.40111 */
/* RX filter BW = 210.937500 */
/* Data format = Normal mode */
/* CRC enable = true */
/* Whitening = false */
/* Device address = 0 */
/* Address config = No address check */
/* CRC autoflush = false */
/* PA ramping = false */
/* TX power = 10 */
RF_SETTINGS rfSettings = {
        0X06,        //IOCFG0      GDO0 Output Pin Configuration
		0X40,        //FIFOTHR     RX FIFO and TX FIFO Thresholds
		0X05,        //PKTCTRL0    Packet Automation Control
		0X06,        //FSCTRL1     Frequency Synthesizer Control
		0X10,        //FREQ2       Frequency Control Word, High Byte
		0XD3,        //FREQ1       Frequency Control Word, Middle Byte
		0X40,        //FREQ0       Frequency Control Word, Low Byte
		0XF8,        //MDMCFG4     Modem Configuration
		0X83,        //MDMCFG3     Modem Configuration
		0X13,        //MDMCFG2     Modem Configuration
		0X15,        //DEVIATN     Modem Deviation Setting
		0X18,        //MCSM0       Main Radio Control State Machine Configuration
		0X16,        //FOCCFG      Frequency Offset Compensation Configuration
		0XFB,        //WORCTRL     Wake On Radio Control
		0XE9,        //FSCAL3      Frequency Synthesizer Calibration
		0X2A,        //FSCAL2      Frequency Synthesizer Calibration
		0X00,        //FSCAL1      Frequency Synthesizer Calibration
		0X1F,        //FSCAL0      Frequency Synthesizer Calibration
		0X81,        //TEST2       Various Test Settings
		0X35,        //TEST1       Various Test Settings
		0X09,        //TEST0       Various Test Settings

};



// PATABLE (+10 dBm output power)
unsigned char paTable[] = {
	0x60 //, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0
	//0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
};



unsigned char ccxxx0_Read(unsigned char addr)
{
	unsigned char x;
	PORTB &= ~(1 << CC_CSN);

	while(PINB & (1 << CC_SO));

	SPDR = (addr | 0x80);
	while(!(SPSR & (1<<SPIF)));
	x = SPDR; // flush SPDR

	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	x = SPDR; //// flush SPDR

	PORTB |= (1 << CC_CSN);

	return x;
}

unsigned char ccxxx0_Write(unsigned char addr, unsigned char dat)
{
	unsigned char x;
	PORTB &= ~(1 << CC_CSN);

	while(PINB & (1 << CC_SO));

	SPDR = addr;
	while(!(SPSR & (1<<SPIF)));
	x = SPDR;// flush SPDR
	
	SPDR = dat;
	while(!(SPSR & (1<<SPIF)));
	x = SPDR; // get data from SPDR

	PORTB |= (1 << CC_CSN);

	return x;
}

unsigned char ccxxx0_Strobe(unsigned char addr)
{
    unsigned char x;
	PORTB &= ~(1 << CC_CSN);

	while(PINB & (1 << CC_SO));

    SPDR = addr;
	while(!(SPSR & (1<<SPIF)));
    x = SPDR; // flush SPDR

    PORTB |= (1 << CC_CSN);

    return x;
}

void ccxxx0_ReadBurst(unsigned char addr, unsigned char* dataPtr, unsigned int dataCount)
{
	unsigned char x;

	PORTB &= ~(1 << CC_CSN);

	while(PINB & (1 << CC_SO));

    SPDR = (addr | 0xc0);
	while(!(SPSR & (1<<SPIF)));
	x = SPDR;// flush SPDR

	while(dataCount) {
	    SPDR = 0;
		while(!(SPSR & (1<<SPIF)));

	    *dataPtr++ = SPDR; // get data from SPDR
		dataCount--;
	}

    PORTB |= (1 << CC_CSN);
}

void ccxxx0_WriteBurst(unsigned char addr, unsigned char* dataPtr, unsigned int dataCount)
{
	unsigned char x;

	PORTB &= ~(1 << CC_CSN);

	while(PINB & (1 << CC_SO));

    SPDR = addr | 0x40;
	while(!(SPSR & (1<<SPIF)));
	x = SPDR; // flush SPDR

	while(dataCount) {
	    SPDR = *dataPtr++;
		while(!(SPSR & (1<<SPIF)));

		dataCount--;
	}

    PORTB |= (1 << CC_CSN);
}

void ccxxx0_PowerOnReset()
{
	unsigned char x;
	//datasheet cc1101 pg on.51
    PORTB |= (1 << CC_CSN);
	_delay_us(1);
	PORTB &= ~(1 << CC_CSN);
	_delay_us(1);
    PORTB |= (1 << CC_CSN);
	_delay_us(41);
	
	PORTB &= ~(1 << CC_CSN);
	
	while(PINB & (1 << CC_SO));
	
	_delay_us(50); 

    SPDR = CCxxx0_SRES;
	while(!(SPSR & (1<<SPIF)));
	x = SPDR; // flush SPDR
	
	while(PINB & (1 << CC_SO));

	_delay_us(50); 
	
    PORTB |= (1 << CC_CSN);
}
void ccxxx0_Setup(const RF_SETTINGS* settings)
{
	unsigned char read;
    // Write register settings
    ccxxx0_Write(CCxxx0_IOCFG0,   settings->IOCFG0);
	read = ccxxx0_Read(CCxxx0_IOCFG0);
	transmit_UART0(read);    
    ccxxx0_Write(CCxxx0_FIFOTHR,  settings->FIFOTHR);
	read = ccxxx0_Read(CCxxx0_FIFOTHR);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_PKTCTRL0, settings->PKTCTRL0);
	read = ccxxx0_Read(CCxxx0_PKTCTRL0);
	transmit_UART0(read);
	ccxxx0_Write(CCxxx0_FSCTRL1,  settings->FSCTRL1);
	read = ccxxx0_Read(CCxxx0_FSCTRL1);
	transmit_UART0(read);
    //ccxxx0_Write(CCxxx0_FSCTRL0,  settings->FSCTRL0);
    ccxxx0_Write(CCxxx0_FREQ2,    settings->FREQ2);
	read = ccxxx0_Read(CCxxx0_FREQ2);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FREQ1,    settings->FREQ1);
	read = ccxxx0_Read(CCxxx0_FREQ1);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FREQ0,    settings->FREQ0);
	read = ccxxx0_Read(CCxxx0_FREQ0);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_MDMCFG4,  settings->MDMCFG4);
	read = ccxxx0_Read(CCxxx0_MDMCFG4);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_MDMCFG3,  settings->MDMCFG3);
	read = ccxxx0_Read(CCxxx0_MDMCFG3);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_MDMCFG2,  settings->MDMCFG2);
	read = ccxxx0_Read(CCxxx0_MDMCFG2);
	transmit_UART0(read);
    //ccxxx0_Write(CCxxx0_MDMCFG1,  settings->MDMCFG1);
    //ccxxx0_Write(CCxxx0_MDMCFG0,  settings->MDMCFG0);
    ccxxx0_Write(CCxxx0_DEVIATN,  settings->DEVIATN);
	read = ccxxx0_Read(CCxxx0_DEVIATN);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_MCSM0 ,   settings->MCSM0 );
	read = ccxxx0_Read(CCxxx0_MCSM0);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FOCCFG,   settings->FOCCFG);
	read = ccxxx0_Read(CCxxx0_FOCCFG);
	transmit_UART0(read);
    //ccxxx0_Write(CCxxx0_BSCFG,    settings->BSCFG);
    ccxxx0_Write(CCxxx0_WORCTRL,  settings->WORCTRL);
	read = ccxxx0_Read(CCxxx0_WORCTRL);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FSCAL3,   settings->FSCAL3);
	read = ccxxx0_Read(CCxxx0_FSCAL3);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FSCAL2,   settings->FSCAL2);
	read = ccxxx0_Read(CCxxx0_FSCAL2);
	transmit_UART0(read);
	ccxxx0_Write(CCxxx0_FSCAL1,   settings->FSCAL1);
	read = ccxxx0_Read(CCxxx0_FSCAL1);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_FSCAL0,   settings->FSCAL0);
	read = ccxxx0_Read(CCxxx0_FSCAL0);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_TEST2,    settings->TEST2);
	read = ccxxx0_Read(CCxxx0_TEST2);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_TEST1,    settings->TEST1);
	read = ccxxx0_Read(CCxxx0_TEST1);
	transmit_UART0(read);
    ccxxx0_Write(CCxxx0_TEST0,    settings->TEST0);
	read = ccxxx0_Read(CCxxx0_TEST0);
	transmit_UART0(read);	
}

void CC_Transmit(unsigned int pkt_length)
{
	PORTC = 0xff;
	char temp[61];
	
	//strcpy((char*)data_transmit, (const char*)address);
	for (int i = 0; i<61; i++)
	{
		data_transmit[i] =  address[i];
	}
	
	/*for (int i = 0; i<35; i++)
	{
		data_transmit[i] =  0xAA;
	}*/
	
	
	//if(data_transmit[34]==0x7E){PORTC = 0xAA;};
	ccxxx0_Strobe(CCxxx0_SIDLE);//Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
	ccxxx0_WriteBurst(CCxxx0_PATABLE, &paTable[0], 1); // max power
	ccxxx0_Strobe(CCxxx0_SFTX); // flush tx buff
	
	ccxxx0_Strobe(CCxxx0_STX); // goto tx mode
	ccxxx0_WriteBurst(CCxxx0_TXFIFO, (unsigned char*)data_transmit,61); // addr=M, payload=4 bytes, Total PKTLEN=5//see cc1101 datasheet pg no.-40
	//ccxxx0_ReadBurst(CCxxx0_TXFIFO, (unsigned char*)data_transmit,35);
	if(data_transmit[60]==0x7E){PORTC = 0xAA;};
	_delay_ms(5);
	//ccxxx0_Strobe(CCxxx0_STX); // goto tx mode
	_delay_ms(75);// initially 120ms
	
	ccxxx0_Strobe(CCxxx0_SIDLE);
	_delay_ms(10);
	PORTC = 0x00;
	//transmit_string_UART0("transmitted Data: ");
	//transmit_string_UART0((unsigned char *)data_transmit);
	//transmit_string_UART0("\r\n");
}

void CC_Receive()
{
	transmit_enable = 1;
	unsigned char temp[30];
	ccxxx0_Strobe(CCxxx0_SIDLE);//Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
	ccxxx0_WriteBurst(CCxxx0_PATABLE, &paTable[0], 1); // max power
	_delay_ms(1);
	ccxxx0_Strobe(CCxxx0_SFRX); // flush rx buff
	ccxxx0_Strobe(CCxxx0_SRX);// goto rx mode
	transmit_string_UART0("Started\r\n");
	while(1)
	{
		char bytes_RXFIFO = ccxxx0_Read(CCxxx0_RXBYTES);
		if(transmit_enable==1)
		{
			transmit_string_UART0("transmitting\r\n");
			CC_Transmit(pkt_length);	
			transmit_check=1;
			//Receive Enable
			ccxxx0_Strobe(CCxxx0_SIDLE);//Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
			ccxxx0_WriteBurst(CCxxx0_PATABLE, &paTable[0], 1); // max power
			_delay_ms(1);
			ccxxx0_Strobe(CCxxx0_SFRX); // flush rx buf
			ccxxx0_Strobe(CCxxx0_SRX); // goto rx mode
			transmit_string_UART0("transmitted\r\n");
		}
			
		
		// If you have a package for us
		
		else if( PINB&(1 << CC_GDO0) )
		{
			transmit_string_UART0("package available \n");
			while(PINB&(1 << CC_GDO0));
			
			ccxxx0_ReadBurst(CCxxx0_RXFIFO, temp, 8);
			transmit_string_UART0("RXed data: ");
			transmit_string_UART0((char *)temp);
			transmit_string_UART0("\r\n");
			_delay_ms(1);
			if (strncmp ((const char *)temp,(const char *)address,6) == 0)
			{
				transmit_string_UART0("address matched...\n");
				if(temp[7]==',')
				{
					transmit_string_UART0((char *)address);
					transmit_UART0(temp[6]);
					transmit_UART0(temp[7]);
					ccxxx0_ReadBurst(CCxxx0_RXFIFO, temp,4);
					transmit_string_UART0((char *)temp);
				}	
				
				else
				{
					transmit_string_UART0((char *)address);
					transmit_UART0(temp[6]);
					transmit_UART0(temp[7]);
					ccxxx0_ReadBurst(CCxxx0_RXFIFO, temp, pkt_length-8);
					transmit_string_UART0((char *)temp);
				}
			}	
			ccxxx0_Strobe(CCxxx0_SIDLE);//Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
			ccxxx0_WriteBurst(CCxxx0_PATABLE, &paTable[0], 1); // max power
			_delay_ms(1);
			ccxxx0_Strobe(CCxxx0_SFRX); // flush rx buf
			ccxxx0_Strobe(CCxxx0_SRX); // goto rx mode
		}
	}		
}
ISR(USART_RXC_vect){
	cli();
	//transmitFlag = 0;
	int i = 0;
	address[i] = UDR;
	//if(address[i]==0x7E){PORTC = 0xCC;}
		
		for(i = 1;i<61; i++){
			char temp = receive_UART0();
			address[i] = temp;}
			//transmit_UART0(temp);}
			//if(address[60]==0x7E){PORTC = 0xEE;}
		address[i] = '\0';
		CC_Transmit(61);
		//transmitFlag = 1;
		sei();
	//transmit_string_UART0("In ISR");
	
	//transmit_string_UART0("Out of ISR");
}
/*****
	MAIN program
*****/
int main(void)
{	
	cli(); 							//Clears the global interrupts			
	ATMEGA_Init();
	sei(); 

	DDRC = 0b00001111;
	PORTC = 0xFF;
	//transmit_string_UART0("cc1101_PowerOnReset\r\n");
	_delay_ms(1000);
	PORTC = 0x00;
	ccxxx0_PowerOnReset();
	//transmit_string_UART0("cc1101_Setup\r\n");
	ccxxx0_Setup(&rfSettings);

		while(1)
		{
			//CC_Transmit(61);
			//_delay_ms(1000);
			
			/*
			if(transmitFlag == 1){
			CC_Transmit(8);
			}
			else {
				transmit_string_UART0("Did not go in ISR!");
			}
			*/
		}
		

	// LOOP HERE FOREVER .. if we ever come here :)
	while (1) { asm("nop"); }

	return 0;
}
