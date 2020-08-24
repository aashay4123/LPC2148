/******************************************************************************

A basic demo program showing sms functions.

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


#include <avr/io.h>
#include <util/delay.h>

#include "lib/lcd/lcd.h"
#include "lib/sim300/sim300.h"


void Halt();
int main(void)
{
	//Initialize LCD Module
	LCDInit(LS_NONE);
	
	//Intro Message
	LCDWriteString("SIM300 Demo !");
	LCDWriteStringXY(0,1,"By Avinash Gupta");
	
	_delay_ms(1000);
	
	LCDClear();
	
	
	//Initialize SIM300 module
	LCDWriteString("Initializing ...");
	int8_t r= SIM300Init();
	
	_delay_ms(1000);
	
	//Check the status of initialization
	switch(r)
	{
		case SIM300_OK:
			LCDWriteStringXY(0,1,"OK !");
			break; 
		case SIM300_TIMEOUT:
			LCDWriteStringXY(0,1,"No response");
			Halt();
		case SIM300_INVALID_RESPONSE:
			LCDWriteStringXY(0,1,"Inv response");
			Halt();
		case SIM300_FAIL:
			LCDWriteStringXY(0,1,"Fail");
			Halt();
		default:
			LCDWriteStringXY(0,1,"Unknown Error");
			Halt();	
	}
	
	_delay_ms(1000);
	
	//IMEI No display
	LCDClear();
	
	char imei[16];
	
	r=SIM300GetIMEI(imei);
	
	if(r==SIM300_TIMEOUT)
	{
		LCDWriteString("Comm Error !");
		Halt();
	}
	
	LCDWriteString("Device IMEI:");
	LCDWriteStringXY(0,1,imei);
	
	_delay_ms(1000);
	
	//Manufacturer ID
	LCDClear();
	
	char man_id[48];
	
	r=SIM300GetManufacturer(man_id);
	
	if(r==SIM300_TIMEOUT)
	{
		LCDWriteString("Comm Error !");
		Halt();
	}
	
	LCDWriteString("Manufacturer:");
	LCDWriteStringXY(0,1,man_id);
	
	_delay_ms(1000);
	
	//Manufacturer ID
	LCDClear();
	
	char model[48];
	
	r=SIM300GetModel(model);
	
	if(r==SIM300_TIMEOUT)
	{
		LCDWriteString("Comm Error !");
		Halt();
	}
	
	LCDWriteString("Model:");
	LCDWriteStringXY(0,1,model);
	
	_delay_ms(1000);
	

	
	//Check Sim Card Presence
	LCDClear();
	LCDWriteString("Checking SIMCard");
	
	_delay_ms(1000);
	
	r=SIM300IsSIMInserted();
	
	if (r==SIM300_SIM_NOT_PRESENT)
	{
		//Sim card is NOT present
		LCDWriteStringXY(0,1,"No SIM Card !");
		
		Halt();
	}
	else if(r==SIM300_TIMEOUT)
	{
		//Communication Error
		LCDWriteStringXY(0,1,"Comm Error !");
		
		Halt();
	}
	else if(r==SIM300_SIM_PRESENT)
	{
		//Sim card present
		LCDWriteStringXY(0,1,"SIM Card Present");
			
		_delay_ms(1000);
	}
	
	//Network search
	LCDClear();
	LCDWriteStringXY(0,0,"SearchingNetwork");
	
	uint8_t		nw_found=0;
	uint16_t	tries=0;
	uint8_t		x=0;
	
	while(!nw_found)
	{
		r=SIM300GetNetStat();
		
		if(r==SIM300_NW_SEARCHING)
		{
			LCDWriteStringXY(0,1,"%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0");
			LCDWriteStringXY(x,1,"%1");
			LCDGotoXY(17,1);
			
			x++;
			
			if(x==16) x=0;
			
			_delay_ms(50);
			
			tries++;
			
			if(tries==600)	
				break;
		}
		else
			break;	
		 
	}
	LCDClear();
	
	if(r==SIM300_NW_REGISTERED_HOME)
	{
		LCDWriteString("Network Found");
	}
	else
	{
		LCDWriteString("Cant Connt to NW!");
		Halt();
	}
	
	_delay_ms(1000);
	
	LCDClear();
	
	//Show Provider Name
	char pname[32];
	r=SIM300GetProviderName(pname);
	
	if(r==0)
	{
		LCDWriteString("Comm Error !");
		Halt();
	}
	
	LCDWriteString(pname);
		
	_delay_ms(1000);
	
	//Send MSG
	LCDClear();
	LCDWriteString("Sending Msg");
	
	uint8_t ref;
	

	r=SIM300SendMsg("+919939XXXXXX","Test",&ref);//Change phone number to some valid value!
	
	if(r==SIM300_OK)
	{
		LCDWriteStringXY(0,1,"Success");
		LCDWriteIntXY(9,1,ref,3);
	}
	else if(r==SIM300_TIMEOUT)
	{
		LCDWriteStringXY(0,1,"Time out !");
	}
	else
	{
		LCDWriteStringXY(0,1,"Fail  !");	
	}
	
	_delay_ms(2000);
	
	//Wait for MSG
	uint8_t id;
	
	UFlushBuffer();
	
	while(1)
	{
		LCDClear();
	
		LCDWriteStringXY(0,0,"Waiting for msg");
	
		x=0;
		int8_t vx=1;
		
		while(SIM300WaitForMsg(&id)!=SIM300_OK)
		{
			LCDWriteStringXY(0,1,"%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0");
			LCDWriteStringXY(x,1,"%1");
			LCDGotoXY(17,1);
			
			x+=vx;
			
			if(x==15 || x==0) vx=vx*-1;
		}
	
		LCDWriteStringXY(0,1,"MSG Received    ");
		
		_delay_ms(1000);
		
		//Now read and display msg
		LCDClear();
		char msg[300];
		
		r=SIM300ReadMsg(id,msg);
		
		if(r==SIM300_OK)
		{
			
			LCDWriteStringXY(0,0,msg);
			
			_delay_ms(3000);
			
		}	
		else
		{
			LCDWriteString("Err Reading Msg !");
			
			_delay_ms(3000);
			
		}
		
		//Finally delete the msg
		if (SIM300DeleteMsg(id)!=SIM300_OK)
		{
			LCDWriteString("Err Deleting Msg !");
			
			_delay_ms(3000);	
		}
		
	}
		
	
	Halt();
}

void Halt()
{
	while(1);
}