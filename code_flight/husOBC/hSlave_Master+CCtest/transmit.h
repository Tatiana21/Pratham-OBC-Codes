/*
 * transmit.h
 *
 * Created: 18-04-2012 01:39:14
 *  Author: Hussain
 */ 


#ifndef TRANSMIT_H_
#define TRANSMIT_H_

void cc1020_transmit_packet(uint8_t *data, uint8_t size);
void cc1020_transmit_bit(uint8_t bit);


/************************************************************************/
char cc1020_transmit_rf_char(uint8_t trans_data);
void cc1020_transmit_rf_packet(uint8_t *trans_buffer); 
/************************************************************************/


#endif /* TRANSMIT_H_ */