/* Author Vineeth Kartha
Date: 20-12-2013

Driver for LCD
*/


#include<lpc214x.h>

void delay(int num)
{
 int i,j;
 for(j=num;j>0;j--)
 	for(i=0;i<60000;i++);
}


unsigned char Busy_Wait()			   //This function checks the busy status of LCD
	{
	 unsigned int temp=0;
	 IO0CLR=(1<<18);
	 IO0CLR=(1<<16);
	 IO0SET=(1<<17);	//READ
	 
	 IO0PIN&=0xFF87FFFF;	  
	 IO0DIR&=0xFF87FFFF;
	 IO0PIN|=0x00400000;
	 
	 do{
	 IO0SET=(1<<18);
	 IO0CLR=(1<<18);;
	 IO0SET=(1<<18);
	 IO0CLR=(1<<18);
	 temp=IO0PIN;
	 }
	 while((temp & 0x00400000)==0x00400000);
	 IO0CLR=(1<<18);
	 IO0CLR=(1<<17);
	 IO0DIR&=0xFF80FFFF;
	 IO0DIR|=0x007F0000;
	 return (0);
	}    

void command(unsigned int cmd)
{
 IO0CLR=(1<<18); //setting enable to low
 IO0CLR=(1<<16); //setting RS to 0
 IO0CLR=(1<<17); //setting RW to 0
 IO0PIN&=0xFF87FFFF;
 IO0PIN|=(cmd&0xF0)<<15; //sending the highest 4 bits of data
 
 IO0SET|=(1<<18);
 IO0CLR|=(1<<18);

 IO0PIN&=0xFF87FFFF;
 IO0PIN|=(cmd&0x0F)<<19;
 IO0SET|=(1<<18);
 while(Busy_Wait());
 IO0CLR|=(1<<18);
 delay(1);
}

void data(unsigned int dt)
{
 IO0CLR=(1<<18); //setting enable to low
 IO0SET=(1<<16); //setting RS to 1
 IO0CLR=(1<<17); //setting RW to 0
 IO0PIN&=0xFF87FFFF;
 IO0PIN|=(dt&0xF0)<<15; //sending the highest 4 bits of data
 IO0SET|=(1<<18);

 IO0CLR|=(1<<18);
 IO0PIN&=0xFF87FFFF;
 IO0PIN|=(dt&0x0F)<<19;
 IO0SET|=(1<<18);

 IO0CLR|=(1<<18);
 delay(1);
}

void string(unsigned char *dat)
{
 while(*dat)
 {
  data(*dat);
  dat++;
 }
}

void initLCD(char * message)
{
	PINSEL0=(0<<22)|(0<<21)|(0<<20)|(0<<19)|(0<<18)|(0<<17)|(0<<16);
	IO0DIR=(1<<22)|(1<<21)|(1<<20)|(1|19)|(1<<18)|(1<<17)|(1<<16);
	command(0x20); //4 bit mode of operation
 	command(0x28);// 2 Line mode	
	command(0x01);//Clear
	command(0x0F);	//cursor Blink
	command(0x06);//Entry mode
	command(0x01); 
 	string(message);
 	command(0xC0);
 	string("V1N337H K4R7H4");
}

