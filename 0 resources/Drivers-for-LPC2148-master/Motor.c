/* Author Vineeth Kartha
Date: 17-12-2013*/

/* The pins to the L293D are connected to  P1.16, P1.19 & P1.17,P1.18*/
/* The motor is controlled by the characters 'f' and 'b' sent from a  hyperterminal*/

#include<lpc214x.h>

#define Fosc            12000000                    
#define Fcclk           (Fosc * 5)                  
#define Fcco            (Fcclk * 4)                 
#define Fpclk           (Fcclk / 4) * 1 

void  Init_UART0(int baud)
{  
   unsigned int Baud16;
   U0LCR = 0x83;		            // DLAB = 1
   Baud16 = (Fpclk / 16) / baud;  
   U0DLM = Baud16 / 256;							
   U0DLL = Baud16 % 256;						
   U0LCR = 0x03;
}

#define M1 0x00090000
#define M2 0x00060000

int main()
{
 char option;
 
 PINSEL0=0x00000005;
 PINSEL1=0x00000000;
 PINSEL2=0x00000000;
 
 IO1DIR = (1<<19) | (1<<18) | (1<<17) | (1<<16);
 
 Init_UART0(9600);
 
 while(1)
 {
  while((U0LSR&0x01)==0);
  option=U0RBR;

  if((char)option=='f')
  {
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x000A0000;
  }
  else if((char)option=='b')
  {
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x00050000;
  }
  else if((char)option=='l')
  {
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x00030000;
  }
  else if((char)option=='r')
  {
   IO1CLR=M1;
   IO1CLR=M2;
   IO1SET=0x000C0000;
  }
  else
  {
   IO1CLR=M1;
   IO1CLR=M2;
  }
 }
}
