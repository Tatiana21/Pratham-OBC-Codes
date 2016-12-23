// For f=14.7456MHz, put UBRRL=191, 
// For f=8.00MHz, put UBBRL=103
/**
 * @file uart.h
 * @brief Contains the various pin/port definitions and procedures for uart.
 */

#ifndef UART_8_32_H_
#define UART_8_32_H_
#define F_CPU 8000000
#include <math.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
//#include "common.h"


void init_UART(void );

void transmit_UART(char data);

uint8_t receive_UART(void);

void transmit_string_UART(char *buffer);

uint8_t isKillCodeRxd(void);

#endif /* UART_8_32_H_ */