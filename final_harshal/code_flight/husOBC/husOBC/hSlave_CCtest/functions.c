/*
 * functions.c
 *
 * Created: 18-04-2012 01:45:07
 *  Author: Hussain
 */ 


#include "common.h"
#include "spi.h"
#include "uart.h"
#include "cc1020.h"
#include "functions.h"

uint8_t x;
char arr[100];

void cc1020_init_dio(void)
{
  cbi(DDR_TRX, DCLK);
  sbi(DDR_TRX, DIO);
  sbi(PORT_TRX, DIO);
}

void init_CC1020 (void)
{  
	char buff[30];
  
	init_SPI_trans(CC1020);
	
	_delay_ms(500);
   
	SetupCC1020PD();
	ResetCC1020();//CC is still in PD (Power_Down mode) after reset
	SetupCC1020Allregisters();
	
	_delay_ms(500);
   
	WakeUpCC1020ToRX(RXANALOG);
	if (CalibrateCC1020(PA_POWER)) 
		transmit_string_UART0("\rRx config done....");
	else
		transmit_string_UART0("\rRx config error....\n\r");

	_delay_ms(500);
	
	WakeUpCC1020ToTX(TXANALOG);
  	if (CalibrateCC1020(PA_POWER)) 
		transmit_string_UART0("\rTx config done....\n\r");
	else 
		transmit_string_UART0("\rTx config error....\n\r");
		
	sprintf(buff,"\n\r STATUS = %x",ReadFromCC1020Register(CC1020_STATUS));			// Expected value = 0xBX
	transmit_string_UART0(buff);
}	
	
int init_CC1020_TX (void)
{  
	sbi(DDR_TRX, DIO);
	sbi(PORT_TRX, DIO);
	WakeUpCC1020ToTX(TXANALOG);													
	return SetupCC1020TX(TXANALOG,PA_POWER);
}

int init_CC1020_RX (void)
{  
	cbi(DDR_TRX, DIO);
	WakeUpCC1020ToRX(RXANALOG);													
	return SetupCC1020RX(RXANALOG,PA_POWER);
}

/****************************************************************************/
/*  This routine writes to a single CC1020 register                         */
/****************************************************************************/

void WriteToCC1020Register(unsigned char addr, unsigned char data)
{
	init_SPI_trans(CC1020);
  
	addr = (addr << 1) | 0x01;
	SPI_transfer(addr);
	SPI_transfer(data);
  
	end_SPI();
}
/****************************************************************************/
/*  This routine reads from a single CC1020 register                        */
/****************************************************************************/

unsigned char ReadFromCC1020Register(unsigned char addr)
{
	init_SPI_trans(CC1020);
  
	char Value;
	addr = (addr << 1) & 0xFE;
	SPI_transfer(addr);
  
	Value = SPI_transfer(0xFF);
  
	end_SPI();
	return Value;
}

/****************************************************************************/
/*  This routine puts the CC1020 into power down mode. Use WakeUpCC1020ToRX */
/*  followed by SetupCC1020RX or WakeupCC1020ToTX followed by SetupCC1020TX */
/*  to wake up from power down                                              */
/****************************************************************************/

void SetupCC1020PD(void)
{
  
  // Put CC1020 into power-down
  WriteToCC1020Register(CC1020_MAIN,0x1F);
  
  x = ReadFromCC1020Register(CC1020_MAIN);
  sprintf(arr, "Read: %x\r\n", x);
  send_preflight(arr, strlen(arr));

  // Turn off PA to minimise current draw
  WriteToCC1020Register(CC1020_PA_POWER,0x00);
}


/****************************************************************************/
/*  This routine resets the CC1020, clearing all registers.                 */
/****************************************************************************/  

void ResetCC1020(void)
{
  // Reset CC1020
  WriteToCC1020Register(CC1020_MAIN, 0x0E);

  // Bring CC1020 out of reset
  WriteToCC1020Register(CC1020_MAIN, 0x1F);
}


/****************************************************************************/
/*  This routine configures all CC1020 registers as per values stored in DefaultConfig433[] */
/****************************************************************************/

void SetupCC1020Allregisters(void)
{
  char counter, value;
  
  for(counter = 0x01; counter <= 0x20; counter++)
  {
    value = DefaultConfig433[counter - 1];				// For Transmitter as well as Receiver
    WriteToCC1020Register(counter, value);
  }
}


/****************************************************************************/
/*  This routine wakes the CC1020 up from PD mode to RX mode                */
/****************************************************************************/

void WakeUpCC1020ToRX(char RXANALOG1)
{
  //volatile int i;

  // Turn on xtal oscillator core
  WriteToCC1020Register(CC1020_MAIN,0x1B);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,RXANALOG1);

  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms.
  //for (i=0x0260; i > 0; i--);
	_delay_ms(3);

  // Turn on bias generator
  WriteToCC1020Register(CC1020_MAIN,0x19);

  // Wait for 150 usec
  //for (int i=0x0010; i > 0; i--);
	_delay_us(150);

  // Turn on frequency synthesiser
  WriteToCC1020Register(CC1020_MAIN,0x11);
}

/****************************************************************************/
/*  This routine wakes the CC1020 up from PD mode to TX mode. CC is still in 
	power down mode 		*/
/****************************************************************************/

void WakeUpCC1020ToTX(char TXANALOG1)
{
  // Turn on xtal oscillator core
  WriteToCC1020Register(CC1020_MAIN,0xDB);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,TXANALOG1);

  // Insert wait routine here, must wait for xtal oscillator to stabilise, 
  // typically takes 2-5ms.
	_delay_ms(3);

  // Turn on bias generator
  WriteToCC1020Register(CC1020_MAIN,0xD9);

  // Wait for 150 usec
	_delay_us(150);		

  // Turn on frequency synthesiser
  WriteToCC1020Register(CC1020_MAIN,0xD1);
  
  x = ReadFromCC1020Register(CC1020_MAIN);
  sprintf(arr, "Read: %x\r\n", x);
  send_preflight(arr, strlen(arr));
  
}

/****************************************************************************/
/*  This routine calibrates the CC1020                                      */
/*  Returns 0 if calibration fails, non-zero otherwise. Checks the LOCK     */
/*  to check for success.                                                   */
/****************************************************************************/

int CalibrateCC1020(char PA_POWER1)
{
  int TimeOutCounter;
  int nCalAttempt;

  // Turn off PA to avoid spurs during calibration in TX mode
  WriteToCC1020Register(CC1020_PA_POWER,0x00);

  // Calibrate, and re-calibrate if necessary:
  for (nCalAttempt = CAL_ATTEMPT_MAX; nCalAttempt > 0; nCalAttempt--) 
  {

    // Start calibration
    WriteToCC1020Register(CC1020_CALIBRATE,0xB4);            // verified with cc1020 datasheet
    
    // Wait for 100 usec. As given errata notes.
    _delay_us(100);

    // Monitor calibration complete bit in STATUS register
    // CAL_COMPLETE bit set to 1 when calibration is complete
    // Waiting time according to register settings is approx 27ms. Ref_freq=fosc/2 and CAL_WAIT = 11
    // We are waiting for 30ms
    for(TimeOutCounter = 30; ((ReadFromCC1020Register(CC1020_STATUS) & 0x80) == 0x00) && (TimeOutCounter > 0); TimeOutCounter--)
      _delay_ms(1);
    
    // Monitor lock
    // LOCK_CONTINUOUS bit in STATUS register is set to 1 when PLL is in LOCK
    for(TimeOutCounter = 30; ((ReadFromCC1020Register(CC1020_STATUS) & 0x10) == 0x00) && (TimeOutCounter > 0); TimeOutCounter--)
      _delay_ms(1);	

    // Abort further recalibration attempts if successful LOCK
    if((ReadFromCC1020Register(CC1020_STATUS) & 0x10) == 0x10) 
		 break;
  }

  // Restore PA setting
  WriteToCC1020Register(CC1020_PA_POWER, PA_POWER1);

  // Return state of LOCK_CONTINUOUS bit
  return ((ReadFromCC1020Register(CC1020_STATUS) & 0x10) == 0x10);
}


/****************************************************************************/
/*  This routine puts the CC1020 into RX mode (from TX). When switching to  */
/*  RX from PD, use WakeupC1020ToRX first                                   */
/****************************************************************************/

/* 
	Carrier Sense Threshold is used in RX mode to determine whether valid data 
	is recieved or not. The DCLK signal is generated only when RSSI is greater 
	than CS threshold. The CS threshold is set using CS_level[4:0] bits in VGA4 
	register.
*/

char SetupCC1020RX(char RXANALOG1, char PA_POWER1)
{
  volatile int TimeOutCounter;
  char lock_status;
  
  // Turn ON DCLK_CS (Carrier Sense Squelch) in RX
  WriteToCC1020Register(CC1020_INTERFACE,ReadFromCC1020Register(CC1020_INTERFACE) | 0x10);


  // Switch into RX, switch to freq. reg A		
  WriteToCC1020Register(CC1020_MAIN,0x11);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,RXANALOG1);

  // Monitor LOCK
  for(TimeOutCounter=LOCK_TIMEOUT; ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0)&&(TimeOutCounter>0); TimeOutCounter--)
  {_delay_ms(1);}

  // If PLL in lock 
  if((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x10)
  {
    lock_status = TRUE;
//bi(PORTA,LED_PLL);
  }
  else	// Recalibrate
  {
    // If recalibration ok
		if(CalibrateCC1020(PA_POWER1))
		{
		lock_status = TRUE;
	//bi(PORTA,LED_PLL);
		// Else (recalibration failed)
		}
	   else
	   {
		lock_status = FALSE;
	//bi(PORTA,LED_PLL);
		}
  }


  // Switch RX part of CC1020 on				// RX coming out of PD at this stage
  WriteToCC1020Register(CC1020_MAIN,0x01);
  
  // Configure LOCK pin to continuous lock status. Active low indicates PLL in lock
  //WriteToCC1020Register(CC1020_LOCK,0x20);
  
  // Configure LOCK pin to indicate carrier sense. Active low when RSSI above threshold
  WriteToCC1020Register(CC1020_LOCK,0x40);

  // Return LOCK status to application
  return (lock_status);
}


/****************************************************************************/
/*  This routine puts the CC1020 into TX mode (from RX). When switching to  */
/*  TX from PD, use WakeupCC1020ToTX first                                  */
/****************************************************************************/

char SetupCC1020TX(char TXANALOG1, char PA_POWER1)
{
  int TimeOutCounter;
  int lock_status;

  // Turn off PA to avoid frequency splatter
  WriteToCC1020Register(CC1020_PA_POWER,0x00);

  // Setup bias current adjustment
  WriteToCC1020Register(CC1020_ANALOG,TXANALOG1);

  // Switch into TX, switch to freq. reg B
  WriteToCC1020Register(CC1020_MAIN,0xC1);     // TX Coming out of Power down at this stage

  // Monitor lock
  // LOCK_CONTINUOUS bit set to 1 when PLL is in LOCK
  for(TimeOutCounter=30; ((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x00)&&(TimeOutCounter>0); TimeOutCounter--)
    _delay_ms(1);
	
  // If PLL in lock 
  if((ReadFromCC1020Register(CC1020_STATUS)&0x10)==0x10)
    lock_status = TRUE;
    
  else	// if PLL is not locked then Recalibrate
  {
    // If recalibration ok
		if(CalibrateCC1020(PA_POWER1))
      lock_status = TRUE;
    else
      lock_status = FALSE;
  }

  // Restore PA setting
  WriteToCC1020Register(CC1020_PA_POWER,PA_POWER1);

  // Turn OFF DCLK_CS (Carrier Sense Squelch) in TX
  WriteToCC1020Register(CC1020_INTERFACE,ReadFromCC1020Register(CC1020_INTERFACE) & (~0x10));
  
  // Configure LOCK pin to continuous lock status. Active low indicates PLL in lock
  WriteToCC1020Register(CC1020_LOCK,0x20);

  // Return LOCK status to application
  return (lock_status);
}

