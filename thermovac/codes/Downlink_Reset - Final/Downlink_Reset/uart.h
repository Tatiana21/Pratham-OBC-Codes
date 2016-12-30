/*
 * uart.h
 *
 * Created: 01-04-2012 00:52:10
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

void transmit_UART0(char data);

uint8_t receive_UART0(void);

void send_preflight(char *data, uint16_t size);

void transmit_string_UART0(char *buffer);

#endif /* UART_H_ */