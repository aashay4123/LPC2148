#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"

void send_string(char *p);//Sends a string of data through UART1
void send_char(char data);
void USARTInit(void);
unsigned char receive_char(void);
void CheckGSM_Response(void);
void flush(void);

int main()
{
	lcd_init();
	USARTInit();
	gotoloc(1,1);
	LCD_WriteString("welcome to gsm");
	soft_delay_ms(500);
//	while(1)
//	{	
		
		LCD_Cmd(0x01);
		soft_delay_ms(100);
			
		
		//sending AT command
		LCD_WriteString("sending AT");
		flush();
		send_string("AT");
		soft_delay_ms(10);
		send_char(0x0d);
		soft_delay_ms(10);
		CheckGSM_Response();
		LCD_WriteString("1");
		soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		
			
		
		
		//ECHO OFF
		gotoloc(1,1);
		LCD_WriteString("sending ECHO");
		flush();
		send_string("ATe0");//response is OK
		soft_delay_ms(10);
		send_char(0x0d);
		soft_delay_ms(10);
		CheckGSM_Response();
		LCD_WriteString("2");
		soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		
				
						
		//SETTING BAUD RATE TO 9600
		LCD_WriteString("sending BAUD");
		flush();
		send_string("AT+IPR=9600");
		soft_delay_ms(10);
		send_char(0x0d);
		soft_delay_ms(10);
		CheckGSM_Response();
		LCD_WriteString("3");
		soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		
		
		
		//TEXT MODE
		gotoloc(1,1);
		LCD_WriteString("sending CMGF");
		flush();
		send_string("AT+CMGF=1");//response is OK
		soft_delay_ms(10);
		send_char(0x0d);
		soft_delay_ms(10);
		CheckGSM_Response();
		LCD_WriteString("4");
		soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		
			
			
		//SENDING MESSAGE
		LCD_WriteString("sending CMGS");
		flush();
		send_string("AT+CMGS=\"+918793730320\"");
		send_char(0x0d);//carriage return
		soft_delay_ms(2000);
		send_string("THIS IS FROM OUR GSM..FIRST MISSION COMPLETE");
		send_char(0x1a);//ctrlZ
		soft_delay_ms(10);
		send_char(0x0d);
		soft_delay_ms(10);
		//CheckGSM_Response();
		gotoloc(2,1);
		LCD_WriteString("5");
		soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
	//	}
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
			break;
		send_char(*p++);
	}
}

unsigned char receive_char()
{
	while(!(U1LSR&0x01));
	return U1RBR;
}	


void CheckGSM_Response()
{
		char buff[16],ch;
		int i=0;
		soft_delay_ms(100);
		
		while(1)
		{
			ch = receive_char();
			buff[i] = ch;
			if((ch==0x0d)&&(i!=0))
			{
				buff[i]='\0';
				break;
			}
			i++;
		}
		gotoloc(2,1);
		LCD_WriteString(buff);
}		

void flush()
{
	char ch_1;
	int j=0;
	while(j<8)
		{
			ch_1= U1RBR;
			j++;
		}
}		

void USARTInit()
{
	PINSEL0 = 0x00050005;  //Select TxD for P0.0 and RxD for P0.1
  U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit  
	U1DLM   = 0x00;
	U1DLL   = 0x61;
	U1FCR	  = 0X07;//ENABLE FIFO	
	U1LCR   = 0x03;//to lock mul  and div value

}


