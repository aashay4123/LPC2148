#include "adc.h"
#include <LPC214x.H>                       /* LPC214x definitions */

/**
*******************************************************************************
  Function Name :init_adc0()

  Description :	Initialises the ADC0  

  Input :	None

  Output :	None

  Note : 
*******************************************************************************
*/	
void init_adc0(void)
{
  PINSEL1 = (PINSEL1 & ~(3 << 25)) | (1 << 24);
}

/**
*******************************************************************************
  Function Name :init_adc1()

  Description :	Initialises the ADC1  

  Input :	None

  Output :	None

  Note : 
*******************************************************************************
*/	
void init_adc1(void)
{

}


/**
*******************************************************************************
	Function Name : adc_read()

	Description   :

	Input         : adc number,channel

	Output        : 10 bit AD value

	Note          :
*******************************************************************************
*/

unsigned short adc_read(unsigned char adc_num, unsigned char ch)
{
  unsigned int i=0;
  
  switch(adc_num)
  {
    case ADC0:
      AD0CR = 0x00200D00 | (1<<ch);    // select channel
      AD0CR |= 0x01000000; // Start A/D Conversion
    
      do
      {
        i = AD0GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion
      break;
    
    case ADC1:
      AD1CR = 0x00200D00 | (1<<ch); // select channel
      AD1CR |= 0x01000000; // Start A/D Conversion
      do
      {
        i = AD1GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion
      break;
  }
 
  return (i >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value
}









