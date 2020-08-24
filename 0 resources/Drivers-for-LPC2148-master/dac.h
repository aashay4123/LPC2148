/* Author Vineeth KArtha
Date: 28-10-2013
A Program to Test DAC Communication on the LPC2148*/
#include<lpc214x.h>
initDAC()
{
	PINSEL1|=0x00080000;
}

toDAC(int value)
{
	DACR=value;
	delay(5);
}
