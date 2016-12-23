/*
 * common.h
 *
 * Created: 31-03-2012 14:40:20
 *  Author: Hussain
 */ 

/**
 *@file  common.h
 *@brief Contains the various constant/macro/struct definitions, clock frequency and general functions
 * for SLAVE 
*/

#ifndef COMMON_H_
#define COMMON_H_

///CPU frequency
#define F_CPU 8000000
#define NULL 0

#include <math.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>


#define sbi(x,y)	(x |= (1<<y))
#define cbi(x,y)	(x &= ~(1<<y))
#define tbi(x,y)	(x ^= (1<<y))

//#define TRUE 1
//#define FALSE 0

/** 
 * @defgroup Modes
 * @brief Modes of operation of satellite
 * @todo Safe and emergency mode details to be obtained and coded
 */
//@{
/// During preflight checks
#define PREFLIGHT 0
/// Normal Mode
#define NOMINAL 1
/// Low power Mode
#define SAFE 2
/// Failure Mode
#define EMERGENCY 3
/// Detumbling Mode
#define DETUMBLING 4
//@}

/**
 *@defgroup Preflight_check
 *@todo Check port and pin for preflight checks 
 */
//@{
///Port for preflight checks
#define DDR_PF DDRA
#define PORT_PF PINA
/// Pin to check for preflight checks mode
#define PIN_PF PA0
//@}

#define DDR_TRX DDRC
#define PORT_TRX PORTC
#define PIN_TRX PINC

#define LOCK	PC5	// Connected to LOCK pin of CC1020
#define DIO		PC7	// Connected to DIO pin of CC1020
#define DCLK	PC6	// Connected to DCLK pin of CC1020
/*
#define LOCK	PE6	// Connected to LOCK pin of CC1020
#define DIO		PE5	// Connected to DIO pin of CC1020
#define DCLK	PE4	// Connected to DCLK pin of CC1020

#define DDR_TRX DDRE
#define PORT_TRX PORTE
#define PIN_TRX PINE
*/

#define DDR_PA DDRE
#define PORT_PA PORTE
#define PIN_PA PINE
#define PA_EN	PE7	// Connected to PA enable TPS

/** 
 * @defgroup TX_region Transmission Regions
 * @brief Defining centre and radius for communication check
 */
//@{
#define IN_LAT 22.5833
#define IN_LON 82.7666
#define IN_RAD 15
#define FR_LAT 48.861101
#define FR_LON 2.352104
#define FR_RAD 6
#define GS_LAT 19.133167
#define GS_LON 72.915144
#define GS_RAD 6
#define IN 1
#define GS 2
#define FR 3
//@}

///Frame Time for the main loop
#define FRAME_TIME 2
#define EEP_FRAME_SIZE 22

static uint32_t read_addr = 0, write_addr = 0;


/************************************************************************/
/************************************************************************/
#define		rf_lock1	'w'
#define		rf_lock2	'x'

#define		RESET	0
#define		OVER	52		//104 bytes in the buffer

enum boolean { FALSE=0, TRUE=1 }; 

enum boolean new_packet_received, data_ack_received, t1_overflow,
		t1_active, t4_overflow, t4_active;

short packet_ready; 
unsigned char dummy, rf_key1, rf_key2;
unsigned char rx_buf[70] ;

volatile char TXBUFFER[20];
volatile char RXBUFFER[20];
int curr_buffer_index, curr_byte;
int no_bytes_tosend;				// Total no. of bytes to be transmitted
int no_databytes_toread;			// No. of Data bytes to read after SOF2 and no. of bytes.
int rxcount;						// Total no. of bits received
/************************************************************************/
/************************************************************************/


#endif /* COMMON_H_ */