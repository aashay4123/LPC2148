/* Author Vineeth KArtha
Date: 28-10-2013
A Program to Test Keypad on the LPC2148*/


/* Connect the columns of the matrix to P1.24-P1.27
and rows to P1.28-P1.31*/

#include<lpc214x.h>
#define ROW 4
#define COL 4

#define time 50

void initKeypad()
{
	PINSEL1=0x00000000;
    PINSEL2=0x00000000;
    IO1DIR=(1<<31)|(1<<30)|(1<<29)|(1<<28);
//IO1DIR=0xF00F0000;
//IO0DIR=(1<<22)|(1<<21)|(1<<20)|(1|19)|(1<<18)|(1<<17)|(1<<16);
//IO1CLR=1<<16|1<<17|1<<18|1<<19;
}
char keypad()
{
    int i=0;
	for(i=0;i<ROW;i++)
	{
        IO1SET=0XF0000000;
        IO1CLR=1<<(i+28);
	if((IO1PIN&0X01000000)==0&&i==0)
	{
		delay(time);
		return ('1');

	}
	if((IO1PIN&0X02000000)==0&&i==0)
	{
		delay(time);
		return('2');
	}
	if((IO1PIN&0X04000000)==0&&i==0)
	{
		delay(time);
		return('3');

	}
	if((IO1PIN&0X08000000)==0&&i==0)  //Correct key
	{
		delay(time);
		return('c');
	}

	if((IO1PIN&0X01000000)==0&&i==1)
	{
		delay(time);
		return('4');
	}
	if((IO1PIN&0X02000000)==0&&i==1)
	{
		delay(time);
		return('5');
	}
	if((IO1PIN&0X04000000)==0&&i==1)
	{
		delay(time);
		return('6');
	}
	if((IO1PIN&0X08000000)==0&&i==1)
	{
		delay(time);
		return('m');
	}

	if((IO1PIN&0X01000000)==0&&i==2)
	{
		delay(time);
		return('7');
	}
	if((IO1PIN&0X02000000)==0&&i==2)
	{
		delay(time);
		return('8');
	}
	if((IO1PIN&0X04000000)==0&&i==2)
	{
		delay(time);
		return('9');
	}
	if((IO1PIN&0X08000000)==0&&i==2)
	{
		delay(time);
		return('u');
	}

	if((IO1PIN&0X01000000)==0&&i==3)	 //cancel
	{
		delay(time);
		return('x');
	}
	if((IO1PIN&0X02000000)==0&&i==3)
	{
		delay(time);
		return('0');
	}
	if((IO1PIN&0X04000000)==0&&i==3)  //Ent
	{
		delay(time);
		return('e');
	}
	if((IO1PIN&0X08000000)==0&&i==3)
	{
		delay(time);
		return('d');
	}
	 }
}
