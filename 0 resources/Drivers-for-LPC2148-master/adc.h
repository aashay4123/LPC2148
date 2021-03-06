/* Author Vineeth KArtha
Date: 28-10-2013
A Program to Test ADC Communication on the LPC2148*/
#include<lpc214x.h>

void initADC()			//This function inits ADC peripheral
{
 //AD0CR=0x00200E00;		// SEL = 1 	ADC0 channel 1	Channel 1
 AD0CR=0x00240200;						// CLKDIV = Fpclk / 1000000 - 1 ;1MHz
						// BURST = 0   // CLKS = 0  // PDN = 1 
 						// START = 1   // EDGE = 0 (CAP/MAT)
 PINSEL1 |= 0x05000000;
} 

int convertData()
{
  int ADC_Data1;
  AD0CR = (AD0CR&0xFFFFFF00)|0x02;			   //Select AD0.1 for conversion
  AD0CR|=(1 << 24);							   //Trigger conversion
  while((AD0DR1&0x80000000)==0);			   //Wait for the conversion to be completed
  ADC_Data1 = AD0DR1;						   //Store converted data
  ADC_Data1 = (int)(ADC_Data1>>8) & 0x00FF;
  return ADC_Data1;
}
