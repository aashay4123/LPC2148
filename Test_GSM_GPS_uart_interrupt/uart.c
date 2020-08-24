#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

void myGSM(void) __irq;

void USARTInit()
{
	PINSEL0 = 0x00000005;  //Select TxD for P0.0 and RxD for P0.1
  U0LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit  
	U0DLM   = 0x00;
	U0DLL   = 0x61;
	U0FCR	  = 0x07;//ENABLE FIFO	
	U0LCR   = 0x03;//to lock mul  and div value
	U0IER   = 0x01;
	VICIntEnable|= (1<<6);
	VICVectCntl0 = (1<<5)|6;
	VICVectAddr0 = (unsigned long)myGSM;
	
}

void send_char_0(char data)
{
	while(!(U0LSR&0x20));
	U0THR=data;
}

void send_string_0(char *p)            //Sends a string of data through UART0
{
	while(1)
	{
		if(*p=='\0')
			break;
		send_char_0(*p++);
	}
}


unsigned char receive_char_0()
{
	while(!(U0LSR&0x01));
	return U0RBR;
}

