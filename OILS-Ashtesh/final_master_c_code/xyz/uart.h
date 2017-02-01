/*
 * uart.h
 *
 * Created: 31-03-2012 14:39:20
 *  Author: Hussain
 */ 

/**
 * @file uart.h
 * @brief Contains the various pin/port definitions and procedures for uart.
 */

#ifndef UART_H_
#define UART_H_

#include "common.h"

void init_UART0(void );

void init_UART1(void );

void transmit_UART1(char data);

void transmit_UART0(uint8_t data);

uint8_t receive_UART1(void);

uint8_t receive_UART0(void);

void transmit_string_UART0(char *buffer);

void transmit_string_UART1(char *buffer);

#endif /* UART_H_ */