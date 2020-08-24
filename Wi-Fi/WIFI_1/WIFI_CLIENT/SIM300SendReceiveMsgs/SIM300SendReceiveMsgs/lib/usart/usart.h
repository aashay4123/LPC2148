/******************************************************************************

                          eXtreme Electronics xAPI(TM)
						  ----------------------------
xAPI is a Powerful but easy to use C library to program the xBoard(TM)
series of AVR development board. The library has support for commonly use tasks
like:-

*LCD interfacing
*MUXED 7 segment displays.
*Remote Control
*Serial Communication
*DC Motor Controls with Speed Control
*Analog Measurement for Sensor Interface.
*Temperature Measurement.
*I2C Communication.
*EEPROM Interface
*Real Time Clock (RTC Interface)
*GSM Module Interface

The APIs are highly documented and easy to use even by a beginner.

For More Info Log On to 
www.eXtremeElectronics.co.in

Copyright 2008-2012 eXtreme Electronics India

                                    USART Core
						           ----------
This module is used for doing serial communication using USART.

                                     NOTICE
									--------
NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN 
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com

*******************************************************************************/

#include <inttypes.h>

#ifndef USART_H
#define USART_H
/****************************************************************************************************/

//Macros
#define RECEIVE_BUFF_SIZE 64
#define USART_RXC_VECT USART_RXC_vect	//This change from mcu to mcu. This is for mega8, mega16, mega32

//#define F_CPU 12000000UL

#define UBRR_VALUE(baud) ((F_CPU/(baud<<4))-1)


//Varriables
volatile char URBuff[RECEIVE_BUFF_SIZE];	//USART Receive Buffer
volatile int8_t UQFront;
volatile int8_t UQEnd;

//Functions
void USARTInit(uint16_t ubrrvalue);

uint8_t UDataAvailable();

char UReadData();
void UWriteData(char data);

void UWriteString(char *str);

void UReadBuffer(void *buff,uint16_t len);
void UFlushBuffer();




/****************************************************************************************************/


#endif


