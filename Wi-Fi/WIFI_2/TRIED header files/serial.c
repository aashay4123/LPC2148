#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"
#include "WiFi.h"
void USARTInit()
{
	PINSEL0 = 0x00050005;  //Select TxD for P0.0 and RxD for P0.1
  U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
   U1DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U1FDR=0x00000083;					//MulVal=8 DivVal=3 
	U1FCR	  = 0X07;//ENABLE FIFO	
	U1LCR   = 0x03;//to lock mul  and div value

}
void send_char(char data)
{
	while(!(U1LSR&0x20));
	U1THR=data;
}

void send_string(char *p)            //Sends a string of data through UART1
{
	while(1)
	{
		if(*p=='\0')
		{
			break;
			}
			else
			{
				send_char(*p++);
	}	}
		
}
unsigned char receive_char()
{
	while(!(U1LSR&0x01));
	return U1RBR;
}
int main(void)
{
	lcd_init();
	USARTInit();
	
	gotoloc(1,1);
	LCD_WriteString("welcome to WIFI");
	soft_delay_us(1000000);
	
}
