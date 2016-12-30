/*
 * mag.h
 *
 * Created: 31-03-2012 15:15:41
 *  Author: Hussain
 */ 

/**
 *@file mag.h
 *@brief Includes various functions for magnetometer.
 */

#ifndef MAG_H_
#define MAG_H_

/**
 * @brief Initializes UART at 9600 bps
 * Uses double baud rate but same bps for slower clocks.
 */
void init_UART_MM(void);

/**
 * @brief Sends magnetometer command
 * @param pointer to data Bytes to be sent
 */
void send_MM_cmd(char *data);

/** @brief Poll the Magnetometer for B readings
*/
void poll_MM(void);
void poll_MM1(void);

/** @brief Receive byte through UART
 *  @return Received Byte
 */
uint8_t receive_MM(void);

#endif /* MAG_H_ */