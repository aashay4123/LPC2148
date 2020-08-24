#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>  		/* required for the isalnum function */
#include<math.h>

#include "lcd.h"

void send_string_0(char *p);//Sends a string of data through UART0
void send_char_0(char data);
void USARTInit(void);
unsigned char receive_char_0(void);
void flush(void);
void CheckGSM_Response(void);

int main()
{
	char ch='\0';
	char fullstring[40];
	char fullstr2[16];
	char fullstr1[16];
	
	
	unsigned int i=0,j,k=0,l=0;
	char mode[2];
	char lat_dir;
	char lon_dir; 
	char *ptr;
	char time_utc[10];
	char lat[12];
	char lon[12];
	
	lcd_init();
	USARTInit();
	LCD_WriteString("Sendg AT commds");
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	LCD_WriteString("AT");
	send_string_0("AT");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
	
	
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	LCD_WriteString("POWER");
	flush();
	soft_delay_ms(200);
	send_string_0("AT+CGPSPWR=1");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
	
	LCD_Cmd(0x01);
	flush();
	soft_delay_ms(200);
	soft_delay_ms(100);
	LCD_WriteString("echo");	
	send_string_0("ATe0");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
		
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	LCD_WriteString("IPR 9600");
	flush();
	soft_delay_ms(200);
	send_string_0("AT+CGPSIPR=9600");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
	
	
	while(1)
	{
		LCD_Cmd(0x01);
		soft_delay_ms(250);
		LCD_WriteString("INF 0");
		soft_delay_ms(100);
		send_string_0("AT+CGPSINF=0");
		soft_delay_ms(5);
		send_char_0(0x0D);
		soft_delay_ms(5);
		do{	
				ch=receive_char_0();
				fullstring[i++] = ch;
			}while((ch != 0x0D)&& (i!=1));
						fullstring[i] = '\0';
						while(l<16)
						{
							fullstr1[j++]=fullstring[l++];
						}
						j=0;
						while(l<30)
						{
							fullstr2[j++]=fullstring[l++];
						}
						j=0;
						LCD_Cmd(0x01);
						soft_delay_ms(100);
						LCD_WriteString(fullstr1);
						soft_delay_ms(2000);
						gotoloc(2,1);
						soft_delay_ms(100);
						LCD_WriteString(fullstr2);
						soft_delay_ms(2000);
								
								i=0,j=0,k=0;
								ptr = fullstring;
								while(*(ptr+j) != 0x2C)// for mode
									{
										mode[k] = *(ptr+j);
										j++;
									}
									mode[1] = '\0';
								j++;
								LCD_Cmd(0x01);
								soft_delay_ms(250);		
									LCD_WriteString("mode:");	
								LCD_WriteString(mode);
								soft_delay_ms(250);
									
								
								while(*(ptr+j) != 0x2C)//for lat
									{
										lat[k++] = *(ptr+j);
										j++;
									}
								j++;
								k=0;
								LCD_Cmd(0x01);
								soft_delay_ms(250);	
								LCD_WriteString("latitude:");	
								LCD_WriteString(lat);
									soft_delay_ms(2000);	
								lat_dir = *(ptr+j);
								j++;
								
								while(*(ptr+j) != 0x2C)//for lon
									{
										lon[k++] = *(ptr+j);
										j++;
									}
								j++;
								k=0;
								LCD_Cmd(0x01);
								soft_delay_ms(250);
								LCD_WriteString("longitude:");	
								LCD_WriteString(lon);
								soft_delay_ms(2000);	
									
								lon_dir = *(ptr+j);
								j++;
								
								while(*(ptr+j)!= 0x2C)
								{j++;}
								j++;
								k=0;	
								while(*(ptr+j)!= 0x2C)// for time
								{
									time_utc[k++] = *(ptr+j);
									j++;
								}
								j++;
								k=0;
								LCD_Cmd(0x01);
								soft_delay_ms(250);
								LCD_WriteString("time");	
								LCD_WriteString(time_utc);
								soft_delay_ms(2000);
								
							soft_delay_ms(1000);
		}	
}

void USARTInit()
{
	PINSEL0 = 0x00000005;//Select TxD for P0.0,0.8 and RxD for P0.1,0.9
  U0LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit  
	U0DLM   = 0x00;
	U0DLL   = 0x61;
	U0FCR	  = 0X07;//ENABLE FIFO	
	U0LCR   = 0x03;//to lock mul  and div value
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

void send_char_0(char data)
{
	while(!(U0LSR&0x20));
	U0THR=data;
}

void send_string_0(char *p)            //Sends a string of data through UART1
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


void send_char_1(char data)
{
	while(!(U1LSR&0x20));
	U1THR=data;
}

void send_string_1(char *p)            //Sends a string of data through UART1
{
	while(1)
	{
		if(*p=='\0')
			break;
		send_char_1(*p++);
	}
}


unsigned char receive_char_1()
{
	while(!(U1LSR&0x01));
	return U1RBR;
}	

void CheckGSM_Response()
{
		unsigned char buff[16],ch;
		int i=0;
		soft_delay_ms(100);
		
		//flush(); to remove the echo response	
		while(1)
		{
			ch = receive_char_0();
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
