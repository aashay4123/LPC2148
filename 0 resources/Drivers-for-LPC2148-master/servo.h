/*
 Author: Vineeth Kartha
 Date : 23-12-2013
 Description: This is the driver for a servo motor it makes use of the PWM block in LPC2148
 PWM output is on P0.0
*/

#include<lpc214x.h>

void initServo()
{
PINSEL0=(1<<1);
PWMPCR=0x0;
PWMPR=14; //Prescale value
PWMMR0=20000;  //Time period for the wave
PWMMR1=1000;  //ON Pulse period
PWMMCR = (1<<1);
PWMLER = (1<<1) | (1<<0);
PWMPCR = (1<<9); // enable PWM output
PWMTCR = (1<<1) ; //Reset PWM TC & PR
PWMTCR = (1<<0) | (1<<3); // enable counters and PWM Mode
}

void rotateServo(int angle)
{
	PWMMR1=angle*10+500;
	PWMLER=1<<1;
}

