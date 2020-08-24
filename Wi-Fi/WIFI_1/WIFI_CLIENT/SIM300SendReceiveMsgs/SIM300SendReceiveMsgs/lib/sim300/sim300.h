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

                                    GSM Core
						           ----------
This module is used for interfacing with Standard GSM Module using the AT command
set.

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


#ifndef SIM300_H_
#define SIM300_H_

//Error List
#define SIM300_OK					 1
#define SIM300_INVALID_RESPONSE		-1
#define SIM300_FAIL					-2
#define SIM300_TIMEOUT				-3

//Status
#define SIM300_NW_REGISTERED_HOME	1
#define SIM300_NW_SEARCHING			2
#define SIM300_NW_REGISTED_ROAMING	5
#define SIM300_NW_ERROR				99
#define SIM300_SIM_NOT_READY		100
#define SIM300_MSG_EMPTY			101

#define SIM300_SIM_PRESENT			1
#define SIM300_SIM_NOT_PRESENT		0

//Low Level Functions
int8_t SIM300Cmd(const char *cmd);

//Public Interface
int8_t	SIM300Init();
int8_t	SIM300CheckResponse(const char *response,const char *check,uint8_t len);
int8_t	SIM300WaitForResponse(uint16_t timeout);
int8_t	SIM300GetNetStat();
int8_t	SIM300IsSIMInserted();
uint8_t SIM300GetProviderName(char *);
int8_t	SIM300GetIMEI(char *);
int8_t	SIM300GetManufacturer(char *);
int8_t	SIM300GetModel(char *);
int8_t	SIM300DeleteMsg(uint8_t i);
int8_t	SIM300WaitForMsg(uint8_t *);
int8_t	SIM300ReadMsg(uint8_t i, char *);
int8_t	SIM300SendMsg(const char *, const char *,uint8_t *);



#endif /* SIM300_H_ */