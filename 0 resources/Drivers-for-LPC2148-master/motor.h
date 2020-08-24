/* Author Vineeth KArtha
Date: 28-10-2013
Driver for Motor with L293D on the LPC2148*/

#include<lpc214x.h>

#define M1 0x00090000
#define M2 0x00060000

void initMotor()
{
	 PINSEL1 = (1<<19) | (1<<18) | (1<<17) | (1<<16);
	 IO1DIR  = (1<<19) | (1<<18) | (1<<17) | (1<<16);
}

void forward()
{
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x000A0000;
}

void backward()
{
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x00050000;
}

void left()
{
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x00030000;
}

void left()
{
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x000C0000;
}
