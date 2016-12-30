/*
 * peripherals.h
 *
 * Created: 31-03-2012 15:18:42
 *  Author: Hussain
 */ 

/**
 * @file peripherals.h
 * @brief Constructs and definitions related to peripherals.
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "common.h"

#define TRUE 1
#define FALSE 0


//uint8_t HM_Data[7];
uint16_t SunSensors[6];

/// Captures GPS reading
struct GPS_reading{
  ///* Position data bytes: Long(4) Unit(mm) ECEF Frame
  int32_t x;
  int32_t y;
  int32_t z;
  
  ///* Velocity data bytes: Long(4) Unit(mm/s) ECEF Frame
  int32_t v_x;
  int32_t v_y;
  int32_t v_z;
  
  ///* Latitude Longitude Altitude
  int32_t lat;
  int32_t lon;
  int32_t alt;
  
  ///* Time data HH:MM:SS DD/MM/YYYY
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  
  uint8_t date;
  uint8_t month;
  uint16_t year;
  
  uint16_t pdop;
  
  
  ///* Stores the time since last GPS reading
  uint16_t time_since_reading;
};

/// Captures Sun sensor readings
struct SS_reading{
  uint16_t reading[6];
};

///Captures Magnetometer Readings
struct MM_reading{
  float B_x;
  float B_y;
  float B_z;
};

/** @brief Power up a peripheral
 */
void power_up_peripheral(uint8_t device);

/** @brief Power down a peripheral
 */
void power_down_peripheral(uint8_t device);

/**
 * @brief Start getting the GPS readings
 */
void read_GPS(void);

/**
 * @brief Read the sunsensor readings from the ADC IC
 */
void read_SS(void);

/**
 * @brief Reads magnetometer through UART
 */
void read_MM (void);

/** @brief Send the data to Preflight computer through UART GPS
 *  @param data bytes pointer
 *  @param Size of data
 */
void send_preflight(char *data, uint16_t size);

///The PWM values of the Torquer along with current direction
struct PWM_values{
  
  uint16_t x;
  uint16_t y;
  uint16_t z;
  
  uint8_t x_dir;
  uint8_t y_dir;
  uint8_t z_dir;
};

///Captures the state of the system
struct state{
  struct GPS_reading gps;
  struct SS_reading ss;
  struct MM_reading mm;
  //struct HM_data hm; 
  struct PWM_values pwm;
};

/**
 * @brief Initialize PWM and the current direction pins
 */
void configure_torquer(void);

/**
 * @brief Set PWM values of the torquer as calculated in the control law
 */
void set_PWM(void);

/**
 * @brief Reset PWM values of the torquer to 0
 */
void reset_PWM(void);

///The variable that defines the current state of the satellite
volatile struct state Current_state;

///Variable for storing the current time sincec equinox in seconds
uint64_t Time;

///Stores the Mode the Satellite is in
uint8_t Mode;
volatile int8_t GPS_done;

#endif /* PERIPHERALS_H_ */