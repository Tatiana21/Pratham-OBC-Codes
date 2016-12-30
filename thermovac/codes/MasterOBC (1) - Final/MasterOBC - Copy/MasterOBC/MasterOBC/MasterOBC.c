/*
 * CFile2.c
 *
 * Created: 11-Dec-14 5:23:00 PM
 *  Author: Harshal Jalan
 */ 

#define F_CPU 8000000
#include "spi.c"
#include "common.h"
#include "comm.h"
#include "util/delay.h"
#include "uart.h"
#include "util/twi.h"
#include "peripherals.h"
#include "mag.h"
#include "avr/sleep.h"
#include "timer.h"
	volatile uint8_t check; //Variable for UART INT vector
	char HM_Data[7]; //Array for HM Data
	uint8_t address=0x20, read=1, write=0;// Variables for I2C
	unsigned char write_data=0xFC, recv_data;//Instruction to be sent to the Power Board. Each bit is for turning one load ON/OFF
	unsigned int counter1 = 0; //Beacon OverCurrent controller
	unsigned int counter2 = 0; //Control OverCurrent controller
	unsigned int counter3 = 0; //GPS OverCurrent controller
	unsigned int counter4 = 0; //Downlink OverCurrent controller
	unsigned int overGS = 0;//Flag to see if the satellite has to start transmitting or not
	unsigned int counterforGS = 0;//Variable to count the number of cycles passed when not over GS. Here GS is only a representation of the Ground Station. It has nothing to do with the actual GS
	unsigned int CyclesToGS = 1350;//Go to transmission mode after these many cycles
	unsigned int CounterInsideGS = 0;//Variable to count the number of cycles passed when over GS
	unsigned int CyclesForDownlink = 300;//No. of cycles when downlink is ON.
	unsigned int CounterForUplink = 150;//No. of cycles when uplink is ON.
	unsigned int UniversalCycles = 1; //Variable that counts the number of loops
	unsigned int StarReceived = 0;  //Variable for controlling wireless block
	unsigned int CyclesToCollectData = 5; //No. of Cycles after which data is to be sent on UART
	unsigned int StartTorquer = 0; //Variable for controlling Torquer
	unsigned int TorquerCycles = 0; //Cycles when torquer is ON
	uint8_t GPS_Received; //Variable for receiving GPS Data
	uint8_t BracketReceived = 0;
	
	
	char GPS_Data[13] = "IamUndefined";// Dummy Lat-Long-Alt data, 12 byte long
	
	void calluart(char d)// UART transmit code written by Ashtesh 
	{
		long fosc=8000000;                //setting the baud rate
		int ubrr=((fosc/9600)/16)-1;
		UBRR0H=(unsigned char)(ubrr>>8);
		UBRR0L=(unsigned char)ubrr;
		UCSR0B|=((1<<RXEN0)|(1<<TXEN0)); //enable the receiver and transmitter
		UCSR0C = (1<<USBS0)|(3<<UCSZ00);                 //2 stop bit
		;//8 bit data
		
		
		
		//while(1)
		while(!(UCSR0A&(1<<UDRE0)));     //waiting for transmit buffer to be clear
		UDR0=d;
		
		
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
	/*uint8_t SPI_transfer(uint8_t transmit_byte)
	{
		SPDR = transmit_byte;
		///Wait for Transmission to complete
		while(!(SPSR & (1<<SPIF)));
		///return received Byte
		return SPDR;
	}*/
	
	void TWI_init_master(void) // Function to initialize master for I2C
	{
		//sei();

		TWSR = 0;
		TWCR = 0;
		TWBR = (F_CPU / 200000UL - 16) / 2; // Bit rate
		//TWSR=(0<<TWPS1)|(0<<TWPS0); // Setting prescalar bits
		// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)

	}

	void TWI_start(void) //Function to send I2C start command
	{
		// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
		TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
		while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgement
	}

	void TWI_repeated_start(void) // Function to send I2C repeated start command. Scarcely used
	{
		// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
		TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
		while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
	}

	void TWI_write_address(unsigned char data)//Function for Master side to send slave address for I2C
	{

		TWDR=data; // Address and write instruction
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT)))// Wait till complete TWDR byte transmitted
		while((TWSR & 0xF8)!= 0x18);  // Check for the acknowledgement

	}

	void TWI_read_address(unsigned char data) //Function for slave side to read address sent by Master
	{
		TWDR=data; // Address and read instruction
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
		while((TWSR & 0xF8)!= 0x40);  // Check for the acknoledgement
	}

	void TWI_write_data(unsigned char data)//Function to write data on I2C data line
	{
		TWDR=data; // put data in TWDR
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
		while((TWSR & 0xF8) != 0x28); // Check for the acknoledgement
	}

	void TWI_read_data(void) //Function to read data from I2C data line
	{
		TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
		while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
		while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
		recv_data=TWDR;//PORTA=recv_data;
		if(UniversalCycles % CyclesToCollectData == 0){transmit_UART0(recv_data);}
		
	}

	void TWI_stop(void)//Function to stop data transmission
	{
		// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
		TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
		while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
	}
	
	void torquer_only_x_positive(void){//Function to turn on only x-direction torquer. These functions are often meddled with. So don't be surprised if the definition doesn't match the desclaration	
		reset_PWM();
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 32786;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
	}

	void torquer_only_y_positive(void)//Function to turn on only y-direction torquer
	{
		reset_PWM();
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 32786;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
	}

	void torquer_only_z_positive(void)//Function to turn on only z-direction torquer
	{
		reset_PWM();
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 32768;
		set_PWM ();
	}
	
	void torquer_all_zeroes(void)//Function to turn off all torquers
	{
		reset_PWM();
		Current_state.pwm.x_dir = 0;
		Current_state.pwm.x = 0;
		Current_state.pwm.y_dir = 0;
		Current_state.pwm.y = 0;
		Current_state.pwm.z_dir = 0;
		Current_state.pwm.z = 0;
		set_PWM ();
	}

	
ISR(USART0_RX_vect) //Interrupt vector for UART. UARTs received from the GPS and the external PC will be handled by this
{
	check = UDR0;
	//transmit_UART0(check);
	if(check == '*'){PORTA = 0x11;//Instruction received to turn on downlink and uplink. Downlink will start immediately and then uplink and then there will be the cycle Normal - Downlink - Uplink
		_delay_ms(5000);
		StarReceived = 1;
		overGS = 1;
		CounterInsideGS = 0;
		BracketReceived = 0;
		//FirstStar = 1;
		//counterforGS = CyclesToGS-1;
	}
	else if(check == '&')//Instruction received to turn off downlink indefinitely. It won't start till we transmit *
	{
		PORTA = 0xCC;
		_delay_ms(5000);
		for(int i =0; i<3; i++){
			SPDR = 0xD1;//Tell slave it's time to go back to normal mode
			while(!(SPSR & (1<<SPIF) ));}
			PORTA = 0xAA;
			TWI_start(); // Function to send start condition
			PORTA=0b11000000;
			TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

			PORTA=0b01100000;
			TWI_write_data(write_data);     // Function to write data in slave
			PORTA=0b10100000;
			TWI_stop();
		StarReceived = 0;
		BracketReceived = 0;
		overGS = 0;
		//CounterInsideGS = 0;
		}
		else if(check == '^')//Instruction received to turn on Torquer. X,Y,Z are turned on serially for 30 seconds
		{
			PORTA = 0xEE;
			_delay_ms(5000);
			StartTorquer = 1;
		}
		else if(check == '(')
		{
			PORTA = 0x00;
			_delay_ms(5000);
			BracketReceived = 1;
			StarReceived = 0;
			overGS = 1;
			CounterInsideGS = 0;
			//FirstStar = 1;
			//counterforGS = CyclesToGS-1;
		}
	
}

ISR(USART1_RX_vect)//ISR for Magmeter UART
{
	uint8_t a =UDR1;
	if (UniversalCycles%CyclesToCollectData == 0) {transmit_UART0(a);}
}
	

int main (void)
{
	//watch_dog(T_POWER);
//Start:
//timer1_init();
init_SPI();                 
init_UART0();               
init_UART_MM();
configure_torquer();
//sei();
DDRA=0xF0;
DDRB |= (1<<PB0)|(1<<PB5);//PB0 and PB5 are slave select pins for Slave OBC and ADC
int j=1;   
_delay_ms(2000);
PORTA=0b11010000; //LED indicator for debugging


TWI_init_master(); // Function to initialize TWI
uint8_t data_t; 
uint8_t data_r;


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

uint16_t val0;//First sunsensor combined 16 bit value
uint16_t val1;
uint16_t val2;
uint16_t val3;
uint16_t val4;
uint16_t val5;
_delay_ms(2000);
//cli();

sei(); //
while(1)
{
	timer_reset_two_sec();
if(overGS==0)          //Check that the satellite does not have to transmit
{	
	//check = receive_UART0();
	//if(check == '*'){PORTA = 0x11;
		// _delay_ms(5000);
		 //StarReceived = 1;
		 //FirstStar = 1;
		 //counterforGS = CyclesToGS-1;
		 //}
		 wdt_enable(WDTO_2S);
		 wdt_reset();
		 
	PORTB |= (1<<PB5); //Set slave select of ADC =1
	PORTB &= ~(1<<PB0);//Set slave select of Slave OBC = 0
	//SPCR |= (1<<SPE);
	write_data&= ~(1<<4);// Turn off both downlink
	write_data&= ~(1<<1);//Turn off uplink
	//write_data |= (1<<1); //Temporary jugaad
	//transmit_UART0('a');
PORTA = 0xAA;
TWI_start(); // Function to send start condition
PORTA=0b11000000;
TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

PORTA=0b01100000;
TWI_write_data(write_data);     // Function to write data in slave
PORTA=0b10100000;
TWI_stop(); // Function to send stop condition
//transmit_UART0('b');

if (UniversalCycles%CyclesToCollectData == 0){transmit_string_UART0("PRA");}
_delay_ms(10); // Delay of 10 mili second
//************************Get HM Data from Power Board*************************************
for(int i=0;i<7;i=i+1) 
{
	TWI_start();
	
	TWI_read_address(address+read); // Function to write address and data direction bit(read) on SDA
	TWI_read_data(); // Function to read data from slave
	HM_Data[i] = recv_data;
	TWI_stop();
}
_delay_ms(10);


	for (uint8_t i =0;i<7;i++) {	
			SPDR = HM_Data[i];
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		PORTB |= (1<<PB0);
		PORTB &= ~(1<<PB5);
		wdt_reset();
		
		//SPCR &= ~(1<<SPE);		
		//****************************OC Check Block *************************************//
		
		if(counter1 == 0) //OC Check for Beacon
		{
			if((HM_Data[6]&(0x80)) == 0)
			{
				counter1 = 1;
				write_data &= ~(1<<7);
				
			}
		}
		
		if((counter1 > 0) && (counter1 < 7))
		{
			counter1 = counter1+1;
		}
		
		if(counter1 == 7)
		{
			counter1 = 0;
			write_data |= (1<<7);
		}
		
		if(counter2 == 0) //OC check for Torquer
		{
			if((HM_Data[6]&(0x40)) == 0)
			{
				counter2 = 1;
				write_data &= ~(1<<6);
				
			}
		}
		
		if((counter2 > 0) && (counter2 < 7))
		{
			counter2 = counter2+1;
		}
		
		if(counter2 == 7)
		{
			counter2 = 0;
			write_data |= (1<<6);
		}
		
		if(counter3 == 0) //OC check for GPS
		{
			if((HM_Data[6]&(0x20)) == 0)
			{
				counter3 = 1;
				write_data &= ~(1<<5);
				
			}
		}
		
		if((counter3 > 0) && (counter3 < 7))
		{
			counter3 = counter3+1;
		}
		
		if(counter3 == 7)
		{
			counter3 = 0;
			write_data |= (1<<5);
		}

		if(counter4 == 0) //OC Check for Downlink
		{
			if((HM_Data[6]&(0x10)) == 0)
			{
				counter4 = 1;
				write_data &= ~(1<<4);
				
			}
		}
		
		if((counter4 > 0) && (counter4 < 7))
		{
			counter4 = counter4+1;
		}
		
		if(counter4 == 7)
		{
			counter4 = 0;
			write_data |= (1<<4);
		}
		//calluart(i);
	//**********************************OC Check Block Ends*********************************************//	
	//**********************************ADC Block Begins************************************************//
	wdt_reset();	
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

	//PORTB |= (1<<PB5);
	//PORTB &= ~(1<<PB0);
	//SPCR &= ~(1<<SPE);
	//PORTA = (valh2<<4)&0xF0;
	if(UniversalCycles%CyclesToCollectData == 0){transmit_string_UART0("PRC");} //PRC = Identifier for Sunsensor data for MATLAB GUI
	PORTA = 0xBB;
	//PORTD = valh2;
	//PORTC = vall2;
	val0=convert(vall1,valh1);
	val1=convert(vall2,valh2);
	val2=convert(vall3,valh3);
	val3=convert(vall4,valh4);
	val4=convert(vall5,valh5);
	val5=convert(vall6,valh6);

	if (UniversalCycles%CyclesToCollectData == 0)
	{
	transmitSunSensorUart(val0);
	transmitSunSensorUart(val1);
	transmitSunSensorUart(val2);
	transmitSunSensorUart(val3);
	transmitSunSensorUart(val4);
	transmitSunSensorUart(val5);
	}	//transmit_UART0(receive_UART0());
	PORTB |= (1<<PB5);
	PORTB &= ~(1<<PB0);
	
	//*********************************ADC Block Ends***********************************//
	//*********************************GPS Block Begins*********************************//
	//wdt_reset();
	if (UniversalCycles%CyclesToCollectData == 0){
	transmit_UART0('P');
	transmit_UART0('R');
	transmit_UART0('B');
	}
	//send_MM_cmd("*00WE\r");
	//send_MM_cmd("*00B\r");
		cli(); //Turn of Global Interrupt so that the code won't enter ISR on reeiving data from GPS
	GPS_Received = receive_UART0();
	if (UniversalCycles%CyclesToCollectData == 0){transmit_UART0(GPS_Received);}
	sei();//Turn on global interrupt
	//***********************************GPS Block Ends************************************//
	//**********************************Magmeter Block begins******************************//
	//wdt_reset();
	if (UniversalCycles%CyclesToCollectData == 0)
	{
	transmit_UART0('P');
	transmit_UART0('R');
	transmit_UART0('D');
	}
	//initMagentometer();
	//poll_MM1();
	
	send_MM_cmd("*00P\r"); //Poll the magmeter for data
	uint8_t p;
	/*for (int i=0;i<6;i=i+1)
	{
		p=receive_MM();
		transmit_UART0(p);
	}
	receive_MM();*/
	//transmitMMUart();
	//torquer_only_x_positive();
	//torquer_only_y_positive();
	//torquer_only_z_positive();
	wdt_disable();
	/*
		counterforGS = counterforGS+1; //Increment the ormal mode counter
		if(counterforGS == CyclesToGS)
		{
			overGS=1;
			counterforGS = 0;
		}*/
		_delay_ms(10);
	
}else if((overGS==1)&&((StarReceived == 1)||(BracketReceived == 1)))
{
	PORTB |= (1<<PB5); 
	PORTB &= ~(1<<PB0);
	//SPCR |= (1<<SPE);
	//write_data&= ~(1<<4);// Turn off both downlink and uplink
	wdt_enable(WDTO_2S);
	wdt_reset();
	PORTA = 0xAA;
	TWI_start(); // Function to send start condition
	PORTA=0b11000000;
	TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

	PORTA=0b01100000;
	TWI_write_data(write_data);     // Function to write data in slave
	PORTA=0b10100000;
	TWI_stop(); // Function to send stop condition

	if(UniversalCycles%CyclesToCollectData == 0){transmit_string_UART0("PRA");}
	_delay_ms(10); // Delay of 10 mili second
	for(int i=0;i<7;i=i+1)
	{
		TWI_start();
		
		TWI_read_address(address+read); // Function to write address and data direction bit(read) on SDA
		TWI_read_data(); // Function to read data from slave
		HM_Data[i] = recv_data;
		TWI_stop();
	}
	//_delay_ms(10);
	wdt_reset();
	
	PORTB |= (1<<PB0);
	PORTB &= ~(1<<PB5);
	//SPCR |=(1<<SPE);
	_delay_ms(1);
	data_r=SPI_transfer(0xFF); //request first data
	_delay_ms(1);
	//transmit_UART0(data_r);
	PORTA = 0xFF;
	vall1=SPI_transfer(0xFF);
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

	//PORTB |= (1<<PB5);
	//PORTB &= ~(1<<PB0);
	//SPCR &= ~(1<<SPE);
	//PORTA = (valh2<<4)&0xF0;
	if (UniversalCycles%CyclesToCollectData == 0) {transmit_string_UART0("PRC");}
	PORTA = 0xBB;
	//PORTD = valh2;
	//PORTC = vall2;
	val0=convert(vall1,valh1);
	val1=convert(vall2,valh2);
	val2=convert(vall3,valh3);
	val3=convert(vall4,valh4);
	val4=convert(vall5,valh5);
	val5=convert(vall6,valh6);

	if (UniversalCycles%CyclesToCollectData == 0)
	{
	transmitSunSensorUart(val0);
	transmitSunSensorUart(val1);
	transmitSunSensorUart(val2);
	transmitSunSensorUart(val3);
	transmitSunSensorUart(val4);
	transmitSunSensorUart(val5);	
	}//transmit_UART0(receive_UART0());
	PORTB |= (1<<PB5);
	PORTB &= ~(1<<PB0);
	wdt_reset();
	if (UniversalCycles%CyclesToCollectData == 0)
	{
	transmit_UART0('P');
	transmit_UART0('R');
	transmit_UART0('B');
	}
	//for(int i=0;i<309;i++){
	cli();
	GPS_Received = receive_UART0();
	if (UniversalCycles%CyclesToCollectData == 0){transmit_UART0(GPS_Received);}
	sei();
	wdt_reset();
	if (UniversalCycles%CyclesToCollectData == 0)
	{
	transmit_UART0('P');
	transmit_UART0('R');
	transmit_UART0('D');
	}
	//initMagentometer();
	//poll_MM1();
	
	
	send_MM_cmd("*00P\r");
	uint8_t p;
	/*for (int i=0;i<6;i=i+1)
	{
		p=receive_MM();
		transmit_UART0(p);
	}
	receive_MM();*/
	//transmitMMUart();
	//torquer_only_x_positive();
	//torquer_only_y_positive();
	//torquer_only_z_positive();
	//wdt_reset();

	if(CounterInsideGS == 0)
	{
	write_data |= (1<<4); //Make downlink 1 in commandbyte and Uplink 0
	wdt_reset();
	//***********Tell Power To Turn On Downlink and Turn Off Uplink**************//
	PORTA = 0xAA;
	TWI_start(); // Function to send start condition
	PORTA=0b11000000;
	TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

	PORTA=0b01100000;
	TWI_write_data(write_data);     // Function to write data in slave
	PORTA=0b10100000;
	TWI_stop();
	//***********Told***************************************//
	}
	if(CounterInsideGS==1) //Time to start downlink
	{  
		if(StarReceived == 1)
		{
		for(int i = 0; i<3; i++){
			SPDR = 0xF1;//Tell slave it's time to start downlink
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);	
		}
		}
		else if(BracketReceived == 1)
		{
			for(int i = 0; i<3; i++){
				SPDR = 0xB1;//Tell slave it's time to start downlink
				while(!(SPSR & (1<<SPIF) ));
				PORTA = 0xFF;
				_delay_ms(1);
			}
		}
	}
	//wdt_reset();
	if((CounterInsideGS >1)&&(CounterInsideGS<(CyclesForDownlink+2)))//Send Lat-Long-Alt
	{
		
		for (uint8_t i =0;i<7;i++) {
			SPDR = HM_Data[i];
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		for (uint8_t i =0;i<12;i++) {
			SPDR = GPS_Data[i];
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		
	}
	//wdt_reset();
	if(CounterInsideGS==(CyclesForDownlink+2))//Time to STOP Downlink transmission
	{
		for(int i=0; i<3; i++){
		SPDR = 0xE1;//Tell slave it's time to stop downlink
		while(!(SPSR & (1<<SPIF) ));
		PORTA = 0xFF;
		_delay_ms(1);}
	}
	
	if(CounterInsideGS==(CyclesForDownlink+3))//Tell Power to turn OFF Downlink and Start Uplink
	{
		write_data&= ~(1<<4); //Make downlink 0 in commandbyte
		write_data|= (1<<1);
		//***********Tell Power To Turn Off Downlink**************//
		PORTA = 0xAA;
		TWI_start(); // Function to send start condition
		PORTA=0b11000000;
		TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

		PORTA=0b01100000;
		TWI_write_data(write_data);     // Function to write data in slave
		PORTA=0b10100000;
		TWI_stop();
		//*******************Told Power to Turn off Downlink*********************//
	}
	//wdt_reset();
	if((CounterInsideGS>(CyclesForDownlink+3))&&(CounterInsideGS<(CyclesForDownlink+4+CounterForUplink)))//Uplink
	{}
	
	if(CounterInsideGS==(CyclesForDownlink+4+CounterForUplink))
	{
		wdt_reset();
		write_data&= ~(1<<4);
		write_data&= ~(1<<1); //Make downlink 0 in commandbyte and also uplink
		
		//***********Tell Power To Turn Off Downlink and Uplink**************//
		PORTA = 0xAA;
		TWI_start(); // Function to send start condition
		PORTA=0b11000000;
		TWI_write_address(address); // Function to write address and data direction bit(write) on SDA

		PORTA=0b01100000;
		TWI_write_data(write_data);     // Function to write data in slave
		PORTA=0b10100000;
		TWI_stop();
		//*******************Told Power to Turn on Downlink*********************//
		
		for(int i =0; i<3; i++){
		SPDR = 0xD1;//Tell slave it's time to go back to normal mode
		while(!(SPSR & (1<<SPIF) ));
		PORTA = 0xFF;
		_delay_ms(1);
		}
		overGS = 0;
	}
	wdt_disable();
	CounterInsideGS = CounterInsideGS+1;
	if(overGS == 0)
	{CounterInsideGS = 0;}
	//_delay_ms(2000);
}
else //If Star is not received and it's time to enter downlink mode
{
	//wdt_enable(WDTO_2S);
	//wdt_reset();
	overGS = 0;
	PORTA = 0x77;
	//_delay_ms(1000);
    //wdt_disable();
}

if(StartTorquer==1)//Block to handle torquer
{
	TorquerCycles = TorquerCycles+1;//Increment torquer cycles in each loop
	if(TorquerCycles <15){torquer_only_x_positive();}//Only X ON for first 15 loops
	else if((TorquerCycles >= 15)&&(TorquerCycles<30)){torquer_only_y_positive();}//Y ON for next 15
	else if((TorquerCycles >= 30)&&(TorquerCycles<45)){torquer_only_z_positive();}//Z ON for next 15
	else if(TorquerCycles>= 45)
	{StartTorquer = 0;
	torquer_all_zeroes();
	 TorquerCycles = 0;}
	
}
//transmit_UART0(UniversalCycles%CyclesToCollectData);

 timer_wait_reset();
 UniversalCycles = UniversalCycles+1; //Increment Universal loop cycles
}

}
		
