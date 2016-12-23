/*
 * functions.h
 *
 * Created: 18-04-2012 01:41:18
 *  Author: Hussain
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void cc1020_init_dio(void);
void init_CC1020(void);
int init_CC1020_TX(void);
int init_CC1020_RX(void);
void WriteToCC1020Register(unsigned char addr, unsigned char data);
unsigned char ReadFromCC1020Register(unsigned char addr);
void SetupCC1020PD(void);
void ResetCC1020(void);
void SetupCC1020Allregisters(void);
void WakeUpCC1020ToRX(char RXANALOG1);
void WakeUpCC1020ToTX(char TXANALOG1);
int CalibrateCC1020(char PA_POWER1);
char SetupCC1020RX(char RXANALOG1, char PA_POWER1);
char SetupCC1020TX(char TXANALOG1, char PA_POWER1);

// Time-out values
#define CAL_TIMEOUT    			30
#define LOCK_TIMEOUT   			30
#define RESET_TIMEOUT  			0x7FFE

#define CAL_ATTEMPT_MAX   4

#endif /* FUNCTIONS_H_ */