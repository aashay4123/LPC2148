/* Author: Vineeth Kartha
Date: 12-12-2013
Description: Timer driver for Timer 0
*/

#include<LPC214x.h>

void initTimer0(void)
{
    /*Assuming that PLL0 has been setup with CCLK = 60Mhz and PCLK also = 15Mhz.*/
   
    T0CTCR = 0x0;
    T0PR = 14; //Step for each counter this is to achieve a microsecond precision (PR+1)/15MHz=delay
    T0TCR = 0x02; //Reset Timer
}

void delayUS(unsigned int microseconds) //Using Timer0
{
    T0TCR = 0x02; //Reset Timer

    T0TCR = 0x01; //Enable timer
   
    while(T0TC < microseconds); //wait until timer counter reaches the desired delay   
    T0TCR = 0x00; //Disable timer
}
