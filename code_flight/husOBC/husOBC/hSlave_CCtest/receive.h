/*
 * receive.h
 *
 * Created: 01-04-2012 03:24:59
 *  Author: Hussain
 */ 


#ifndef RECEIVE_H_
#define RECEIVE_H_

uint8_t cc1020_receive_packet(uint8_t *data);
uint8_t cc1020_receive_bit(void);


/************************************************************************/
char cc1020_receive_rf_char(void);
char cc1020_receive_rf_packet(uint8_t *recv_buffer); 
/************************************************************************/


#endif /* RECEIVE_H_ */