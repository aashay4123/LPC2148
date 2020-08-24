/* Author Vineeth KArtha
Date: 28-10-2013
A Program to Test UART0 Communication on the LPC2148*/

#include<lpc214x.h>
#include<my_header.c>

#define Brate 9600

void  Init_UART(int baudrate)					
{  
   unsigned int Baud16;
   U0LCR = 0x83;		            // DLAB = 1
   Baud16 = (Fpclk / 16) / baudrate;  
   U0DLM = Baud16 / 256;							
   U0DLL = Baud16 % 256;	
   U0LCR = 0x03;					
}
void senddata(unsigned char data)
{
  U0THR=data;
  while((U0LSR&0x40)==0);
}

void sendstring(char *str)
{
  while(*str!='\0')
  senddata(*str++);
}

unsigned int receivedata()
{
  unsigned int data;
  data=U0RBR;
  while((U0LSR&0x01)==0);
  return data;
}

int main()
{
PINSEL0=0x00000005;
PINSEL1=0x00000000;
PINSEL2=0x00000000;
IO0DIR=0x00000000;
Init_UART(Brate);
sendstring("Hello\n");
while(1)
{
if(!(IO0PIN&0x00008000))
	senddata('a');
if(!(IO0PIN&0x00002000))
	senddata('b');
if(!(IO0PIN&0x00001000))
	senddata('c');
if(!(IO0PIN&0x40000000))
	senddata('d');
}
return 0;
}

