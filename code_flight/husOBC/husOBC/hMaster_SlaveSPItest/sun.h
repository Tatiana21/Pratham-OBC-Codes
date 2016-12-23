/*
 * sun.h
 *
 * Created: 31-03-2012 15:21:51
 *  Author: Hussain
 */ 

/**
 * @file sun.h
 * @brief Functions to read the sunsensor
 */

#ifndef SUN_H_
#define SUN_H_

/** @brief Configure the Sun sensor ADC IC
 */
void configure_SS(void);

/** @brief Poll the ADC IC for sun sensor readings
 */
void poll_SS(void);

#endif /* SUN_H_ */