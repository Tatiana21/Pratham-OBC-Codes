/*
 * common.h
 *
 * Created: 01-04-2012 00:53:29
 *  Author: Hussain
 */ 

/**
 *@file  common.h
 *@brief Contains the various constant/macro/struct definitions, clock frequency and general functions
 * for ATmega8 
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

#define TRUE 1
#define FALSE 0

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


#define LOCK	PD7	// Connected to LOCK pin of CC1020
#define DIO		PD6	// Connected to DIO pin of CC1020
#define DCLK	PD3	// Connected to DCLK pin of CC1020

#define DDR_TRX DDRD
#define PORT_TRX PORTD
#define PIN_TRX PIND

#define DDR_LNA DDRB
#define PORT_LNA PORTB
#define PIN_LNA PINB
#define LNA_EN	PB0	// Connected to LNA enable TPS

#define DDR_PA DDRC
#define PORT_PA PORTC
#define PIN_PA PINC
#define PA_EN	PC0	// Connected to PA enable TPS

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

#endif /* COMMON_H_ */

