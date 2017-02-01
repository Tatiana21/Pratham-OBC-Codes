/**
 * @file master.c
 * @brief Main file for master uC.
 * 
 * Contains the scheduler, preflight check routines, high level task blocks like power, control and communication.
 */
#define F_CPU 8000000
#include "common.h"
#include "spi.h"
#include "mathutil.h"
#include "timer.h"
#include "peripherals.h"
#include "mag.h"
//#include "gps.h"
//#include "hm.h"
//#include "slave_comm.h"
#include "comm.h"
#include "controller.h"
#include "util/twi.h"
#include "uart.h"

/// @todo Write Error checking codes to ensure the OBC does not go into infinite loops

/**
 * @defgroup global_state Global State parameters
 */
//@{
 ///////////////////////////////////////////////////
 // initialize timer, interrupt and variable
 // set max_time for cycle at 8.191 ms
 #define T 8.191
 // set min_time (ms) assuming prescalar=1
 #define DT 1.25e-4
 volatile uint8_t tot_overflow;
 void timer1_init()
 {
	 // set up timer with prescaler = 1
	 TCCR1B |= (1 << CS10);
	 
	 // initialize counter
	 TCNT1 = 0;
	 
	 // enable overflow interrupt
	 TIMSK |= (1 << TOIE1);
	 
	 // enable global interrupts
	 sei();
	 
	 // initialize overflow counter variable
	 tot_overflow = 0;
 }

 // TIMER1 overflow interrupt service routine
 // called whenever TCNT1 overflows
 ISR(TIMER1_OVF_vect)
 {
	 // keep a track of number of overflows
	 tot_overflow++;
	 
 }

 //////////////////////////////////////////////////
volatile uint8_t GPS_done = -1;
uint8_t Mode = DETUMBLING;
uint8_t Mode_prev = DETUMBLING;
uint64_t Time;
volatile struct state Current_state;
unsigned char write_data=0x4C;// OBC, Torquer, Magmeter
unsigned int CyclesToCollectData = 1;
unsigned char recv_data;
uint8_t address=0x20, read=1, write=0;
unsigned int UniversalCycles = 1;
unsigned int counter1 = 0; //Beacon OverCurrent controller
unsigned int counter2 = 0; //Control OverCurrent controller
unsigned int counter3 = 0; //GPS OverCurrent controller
unsigned int counter4 = 0; //Downlink OverCurrent controller
char HM_Data[7]; //Array for HM Data
uint8_t FirstTimeOuter = 0;
uint8_t FirstTimeInner = 0;
uint8_t FirstTimeNormal = 0;
char GPS_Data[13] = "IamUndefined";
uint8_t countd=0,countu=0;
uint8_t light_main = 1;
uint8_t flag_india = 0;
uint8_t flag_france = 0; 
uint8_t flag_mumbai = 0;


//@}

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
	//if(UniversalCycles % CyclesToCollectData == 0){transmit_UART0(recv_data);}
	
}

void TWI_stop(void)//Function to stop data transmission
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}



/**
 * @brief Communication with Power
 *
 * Obtains the health monitoring data by communcicating with Power microcontroller.
 */
/*void power(void){
  /// Start watchdog for power tasks
  watch_dog(T_POWER);
  
  ///Every 1.5 minutes get health monitoring data from the power uC
  if(Time % 90 == 0)
  {
    //get_HM_data();
  }
}*/

/**
 * @brief Main function
 */

 void SendHM(void)
 {
	 for (uint8_t i =0;i<7;i++) {
		 SPDR = HM_Data[i];
		 while(!(SPSR & (1<<SPIF) ));
		 PORTA = 0xFF;
		 _delay_ms(1);
	 }
 }


int main(void){
    
  /// Initialise Interfaces - UART of Magnetometer and GPS and the SPI bus
   //0 - no over current; 1-reverse
  //Current_state.gps.gps_OC = 0;
  init_SPI();
  init_UART0();
  init_UART_MM();
  configure_torquer();
  DDRA=0xF0;
  Current_state.gps.gps_OC = 0; // This is important because the default value of OC should be 0
  DDRB |= (1<<PB0)|(1<<PB5);//PB0 and PB5 are slave select pins for Slave OBC and ADC
  int j=1;
  _delay_ms(2000);
  PORTA=0b11010000; //LED indicator for debugging
  TWI_init_master();
  //transmit_string_UART0("Flight");// uncomment afetrwards
  //transmit_string_UART0("Code");
    ///Wait for all components to switch on
    _delay_ms(5000);
    
    ///* Switch on Global interrupts
    sei();
    
    ///* Set default mode of Satellite
    //Mode = DETUMBLING;
   // Mode = NOMINAL;
    ///* initialise Timer
    Time = 0;
	 
    ///* * Reset timer for 2 seconds
   // timer_reset_two_sec();
    ///Loop begins
     //while(1){         // while(!(PORT_PF & _BV(PIN_PF))){
  //transmit_UART0(42);
      /**
      * * * * Task 1: Control codes
       * @ref control
       */
	 
	  
  while (1)
  {
	  /////////////////////
	  timer1_init();
	  uint16_t counter_value=TCNT1;
	  uint8_t overflow=tot_overflow;
	  ///////////////////
	  timer_reset_two_sec();
	  //wdt_enable(WDTO_2S);
	  //wdt_reset();
	  if (Mode == DETUMBLING){ write_data|=(1<<6)|(1<<3)|(1<<2);}
	  if (Mode == NOMINAL){write_data|=(1<<6)|(1<<3)|(1<<2)|(1<<7);}
	  
  PORTB |= (1<<PB5); //Set slave select of ADC =1
	PORTB &= ~(1<<PB0);//Set slave select of Slave OBC = 0
	//SPCR |= (1<<SPE);
	//write_data&= ~(1<<4);// Turn off downlink
	//write_data&= ~(1<<1);//Turn off uplink
	if((Current_state.gps.gps_power_main==2)&&(Current_state.gps.gps_OC == 0))// gps_oc =0 means no overcurrent
	{write_data |= (1<<5);}
		else
		{write_data&= ~(1<<5);}//transmit_UART0('a');
			
	
	if ((Mode == NOMINAL)&&(light_main == 0)) //Nominal+Eclipse
	{
		write_data &= ~(1<<5);// Turn off GPS
		write_data &= ~(1<<2);// Turn off Mag
		write_data &= ~(1<<6);// Turn off Torq
	}
	
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
//transmit_UART0('b');

//if (UniversalCycles%CyclesToCollectData == 0){transmit_string_UART0("PRA");}
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
wdt_disable();
_delay_ms(10);
////////////////////////////////////////////////////////
uint8_t G1,G2,G3,G4,G5,G6,G7,G8,G9,G10,G11,G12;
G1 = (uint8_t)Current_state.gps.lat;
G2 = (uint8_t)((Current_state.gps.lat)>>8);
G3 = (uint8_t)Current_state.gps.lon;
G4 = (uint8_t)((Current_state.gps.lon)>>8);
G5 = Current_state.gps.seconds;
G6 = Current_state.gps.minutes;
G7 = Current_state.gps.hours;
G8 = Current_state.gps.date;
G9 = (uint8_t)(q_o[0]*127);
G10 = (uint8_t)(q_o[1]*127);
G11 = (uint8_t)(q_o[2]*127);
G12 = (uint8_t)(q_o[3]*127);

GPS_Data[0] = G1;
GPS_Data[1] = G2;
GPS_Data[2] = G3;
GPS_Data[3] = G4;
GPS_Data[4] = G5;
GPS_Data[5] = G6;
GPS_Data[6] = G7;
GPS_Data[7] = G8;
GPS_Data[8] = G9;
GPS_Data[9] = G10;
GPS_Data[10] = G11;
GPS_Data[11] = G12;
//////////////////////////////////////////

///////////////////////////////////////
if((flag_india == 1)||(flag_france == 1)) //Check for Circles
{
	
	if(FirstTimeOuter ==0)
	{
		
	FirstTimeInner = 0;
	FirstTimeNormal = 0;
	write_data |= (1<<4);
	write_data &= ~(1<<1);
	FirstTimeOuter = 1;
	}
	else if(FirstTimeOuter == 1)
	{
		
		if(countu ==0)
		{
		
		for(int i = 0; i<3; i++)
		{
			SPDR = 0xB1;//Tell slave it's time to start downlink
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		FirstTimeOuter =2;
		}
		else if(countu == 1)
		{
			
			SendHM();
		}	
	
		
	}
	else
	{
		
		SendHM();// add an if for sending gps data based on flag
		if(countu ==0)
		{
			
			for (uint8_t i =0;i<12;i++) 
			{
				SPDR = GPS_Data[i];
				while(!(SPSR & (1<<SPIF) ));
				PORTA = 0xFF;
				_delay_ms(1);
			}   
		}
	}
	countd=countd+2;
}
else if(flag_mumbai ==1) //Check for Circles
{
	
	if(FirstTimeInner ==0)
	{
		
		FirstTimeOuter = 0;
		FirstTimeNormal = 0;
		write_data |= (1<<4);// if uplink on this is negated below
		write_data &= ~(1<<1);
		FirstTimeInner = 1;
		
	}
	else if(FirstTimeInner == 1)
	{
		
		if(countu ==0)
		{
			
		for(int i = 0; i<3; i++)
		{
			SPDR = 0xF1;//Tell slave it's time to start downlink
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		FirstTimeInner =2;	
		}
		
		else if(countu == 1)
		{
			
			SendHM();
		}
		
	}
	else
	{
		
		SendHM();
		if(countu ==0)
		{
			
		for (uint8_t i =0;i<12;i++)
		{
			SPDR = GPS_Data[i];
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);
		}
		}
	}
	countd=countd+2;
}
else
{

	countd = 0;
	countu = 0;
	if(FirstTimeNormal == 0)
	{
		for(int i =0; i<3; i++){
			SPDR = 0xD1;//Tell slave it's time to go back to normal mode
			while(!(SPSR & (1<<SPIF) ));
			PORTA = 0xFF;
			_delay_ms(1);}
			FirstTimeNormal = 1;
	}
	
	if(UniversalCycles%10==0){SendHM();}// In normal mode, HM in EEPROM every 20 sec, otherwise, every 2 sec
	write_data &= ~(1<<4);
	write_data |= (1<<1); 
	FirstTimeInner = 0;
	FirstTimeOuter = 0;
}

if ((countd >120)&&(countd <=150))
{
	
	write_data &= ~(1<<4); //switch off downlink
	write_data |= (1<<1); // switch on uplink
	if (countd == 122)
	{
	for(int i =0; i<3; i++)
	{
		SPDR = 0xD1;//Tell slave it's time to go back to normal mode
		while(!(SPSR & (1<<SPIF) ));
		PORTA = 0xFF;
		_delay_ms(1);
	}//send normal mode commands d1
	}
	countu = 1;//set a flag 
}
if (countd >150)
{

	countd=0;
	countu=0;
	write_data &= ~(1<<1); 
	write_data |= (1<<4);
	FirstTimeOuter = 0;
	FirstTimeInner = 0;
		
}

	
		PORTB |= (1<<PB0);
		PORTB &= ~(1<<PB5);
     	
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
		 
		
		 
		 
		 
			 if((HM_Data[6]&(0x20)) == 0) //OC check for GPS
			 {
				Current_state.gps.gps_OC = 1;
			}
			else
			{
				Current_state.gps.gps_OC = 0;
			}


	wdt_enable(WDTO_2S);
	wdt_reset();
	control();
	wdt_disable();
	for (int i=0;i<12;i++)
	{
		transmit_UART0(GPS_Data[i]);
	}
	//int w = 2;Current_state.gps.lat = 168;Current_state.gps.lon = 96;
	//flag_india = (int8_t)(Current_state.gps.lat);//(pow((Current_state.gps.lat - 22.5833),2) + cos(Current_state.gps.lat*3.141592/180)*pow((Current_state.gps.lon - 82.7666),2))*255/70000;
	transmit_UART0(flag_india);
	transmit_UART0(flag_mumbai);
	transmit_UART0(flag_france);
	for (int i = 0; i<7;i++)
	{
		transmit_UART0(HM_Data[i]);
	}
	
	light_main = light_cal();	
	 if((HM_Data[6]&(0x40)) == 0) // OC Check for Torquer
	 {
		 reset_PWM();
		 
	 }		
	 
	 //transmit_UART0(Current_state.gps.gps_OC);
     // PORTA=0xf0;
      /**
      * * * * Task 2: Communication with power uC through I2C. @ref power
      */
     // power();

      /**
      * * * * Task 3: Communication check routine;
      * @ref comm
      */
      //comm();
      
      ///* * Increment the Timer
      Time += FRAME_TIME;
	  // for india outer circle
	double a = ((double)Current_state.gps.lat/100.0) - 22.5833;
	double b = ((double)Current_state.gps.lon/100.0) - 82.7666;
	double c = ((double)Current_state.gps.lat/100.0)*3.141592/180;
// for france
	double a1 = ((double)Current_state.gps.lat/100.0) - 48.8;
	double b1 = ((double)Current_state.gps.lon/100.0) - 2.33;
	double c1 = ((double)Current_state.gps.lat/100.0)*3.141592/180;
// for india inner circle
	double a2 = ((double)Current_state.gps.lat/100.0) - 19.076;
	double b2 = ((double)Current_state.gps.lon/100.0) - 72.877;
	double c2 = ((double)Current_state.gps.lat/100.0)*3.141592/180;
		  // over mumbai
		if (((a2*a2 + cos(c2)*b2*b2) < 100) && (Mode == NOMINAL))//change it to nominal  ((HM_Data[1])>163)
		{
			flag_mumbai = 1;
			flag_india = 0;
			flag_france = 0;
		}  
		//over india outer
      else if (((a*a + cos(c)*b*b) < 625) && (Mode == NOMINAL))//change it to nominal && ((HM_Data[1])>163) 
	  {
      flag_mumbai = 0;
      flag_india = 1;
	  flag_france = 0;
	  }
	  //over france
      else if (((a1*a1 + cos(c1)*b1*b1) < 100)  && (Mode == NOMINAL))//&& ((HM_Data[1])>163)
      {
	  flag_mumbai = 0;
      flag_france = 1;
	  flag_india = 0;
	  }
	  // normal mode no downlink
      else
	  {
      flag_france = 0;
	  flag_india = 0;
	  flag_mumbai = 0;
	  }
      ///* * Wait for 2 seconds to get over
     // timer_wait_reset();
	  /*if (UniversalCycles%600 < 152)
	  {
		  Current_state.gps.lat = 1000; //Outer circle
		  Current_state.gps.lon = 1000;
		  transmit_UART0('I');
	  }
	  else if((UniversalCycles%600 >= 152)&&(UniversalCycles%600 <300))
	  {
		Current_state.gps.lat = 2000;//Inner Circle
		Current_state.gps.lon = 2000;
		transmit_UART0('O');  
	  }
	   else if((UniversalCycles%600 >= 300)&&(UniversalCycles%600 <450))
	   {
		   Current_state.gps.lat = 1000; //Inner Circle
		   Current_state.gps.lon = 1000;
		   transmit_UART0('S');
	   }
	   else if((UniversalCycles%600 >= 450))
	   {
		   Current_state.gps.lat = 0; //Outside both circles
		   Current_state.gps.lon = 0;
		   transmit_UART0('N');
	   }
      */
      //_delay_ms(200);
	  UniversalCycles = UniversalCycles+1;
    //}
	//////////////////////////////////
	overflow=tot_overflow-overflow;
	counter_value = TCNT1 + (65535-counter_value);
	uint32_t net_time =  (counter_value*DT + overflow*T)*1e3;
	transmit_UART0((uint8_t)(net_time));
	transmit_UART0((uint8_t)(net_time>>8));
	transmit_UART0((uint8_t)(net_time>>16));
	transmit_UART0((uint8_t)(net_time>>24));
	transmit_UART0('\r');
	//////////////////////////////////
	timer_wait_reset();
	//wdt_disable();
  }
  return 0;
}
