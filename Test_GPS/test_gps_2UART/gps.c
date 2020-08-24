#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>  		/* required for the isalnum function */
#include<math.h>

#include "lcd.h"

void send_string_0(char *p);//Sends a string of data through UART0
//void send_string_1(char *p);//Sends a string of data through UART1
void send_char_0(char data);
//void send_char_1(char data);
void USARTInit(void);
unsigned char receive_char_1(void);
unsigned char receive_char_0(void);
void flush(void);
void CheckGSM_Response(void);

int main()
{
	unsigned char ch;
	unsigned int i,j,k,numLinesRead;
	unsigned char	 mainstring[100]; 		/* Buffer collects chars read from GPS */
  unsigned char  tempString[100];
  unsigned char  time_string[12];
  unsigned char  latitudestring[15];
  unsigned char  latitudeCardinalString[3];
  unsigned char  longitudestring[12];
  unsigned char  longitudeCardinalString[3];

  unsigned char  *pChar;
  
  unsigned  long utcTime, istTime;		/* Coordinated Universal Time and IST */
  unsigned  long utcHour, istHour;
  unsigned  long utcMinutes, istMinutes;
  unsigned  long utcSeconds, istSeconds;
  unsigned  char lastCommaPosition;

  float latitude;
  int 	latDegrees;
  float latMinutes;

  float longitude;
  int		longDegrees;
  float	longMinutes;


	lcd_init();
	USARTInit();
	LCD_WriteString("sendg AT commds");
	
	send_string_0("AT");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	
	/*send_string_0("AT+CMGF=1");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	*/
	gotoloc(1,1);
	LCD_WriteString("pwr");
	
	send_string_0("AT+CGPSPWR=1");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
	
	
	
	gotoloc(1,1);
	LCD_WriteString("rst");	
	send_string_0("AT+CGPSRST=0");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	soft_delay_ms(500);
	gotoloc(2,1);
	LCD_WriteString("      4");
	
	
	gotoloc(1,1);
	LCD_WriteString("iPR");
	send_string_0("AT+CGPSIPR=9600");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(10);
	CheckGSM_Response();
	LCD_Cmd(0x01);
	gotoloc(2,1);
	LCD_WriteString("      5");
	soft_delay_ms(500);
	
	
	while(1)
	{
		gotoloc(1,1);
	  LCD_WriteString("1111111111111");
		soft_delay_ms(500);
		send_string_0("AT+CGPSINF=2");
	  soft_delay_ms(10);
	  send_char_0(0x0d);
	  soft_delay_ms(10);
		do {
      	ch = receive_char_1();  	// read char from serial port 
      	if(ch == '$') // GPS messages start with $ char
					{     
						i = 0;
						numLinesRead++;
						mainstring[i] = ch;
						do 
							{
								ch = receive_char_1();
								if((ch!='\0')&&(isalnum(ch)||isspace(ch)||ispunct(ch))) 
								{
									i++;
									mainstring[i] = ch;
								}
							} while(ch != 0x0D);
						
							
							
						//By this point, a complete GPS string has been read so save it to file
	  	  		mainstring[i+1] = '\0';
	  	  		j = 0;
						pChar = mainstring;
						
							
							
						while(*(pChar+j) != 0x2C)//ascii of comma 
						{
							tempString[j] = *(pChar+j);
							j++;
						}
						tempString[j] = '\0';
					
						
						
						// Check if string we collected is the $GPGGA message 
						if(tempString[3] == 'G'&&tempString[4]=='G'&&tempString[5] == 'A')
						{
								pChar = mainstring;     			/* Get UTC time */
								j = 7;  /* start of time field */
								k = 0;
								while(*(pChar+j) != 0x2C) 
								{
									time_string[k] = *(pChar+j);
									j++;
									k++;
								}
								lastCommaPosition = j;
								time_string[k] = '\0';
								sscanf(time_string, "%ld", &utcTime);//READS FROM timestring and copies to utcTime in long float
								/*Time is in HHMMSS.SS format*/
								utcHour = (utcTime/10000);   /* extract Hours from long */
								utcMinutes = (utcTime - (utcHour*10000))/100;  /* extract minutes from long */
								utcSeconds = utcTime - (utcHour*10000) - (utcMinutes*100); /*extract seconds from long */
								if(utcHour >= 0 && utcHour <= 18) 
										istHour = utcHour +5; /*since its 5 hts ahead of utc*/
								else 
										istHour = utcHour -19;
								istMinutes = utcMinutes+30;
								istSeconds = utcSeconds;
								gotoloc(1,1);
								LCD_WriteString("Time:Hour");
								LCD_Write_DATALONG(istHour);
								gotoloc(2,1);
								LCD_WriteString("Min:");
								LCD_Write_DATALONG(istMinutes);
								LCD_WriteString("Sec:");
								LCD_Write_DATALONG(istSeconds);
								soft_delay_ms(2000);
								LCD_Cmd(0x01);
						



								/* Get lattitude: ddmm.mmmm */
								pChar = mainstring;
								j = lastCommaPosition + 1;
								k = 0;
								while(*(pChar+j) != 0x2C) 
								{
									latitudestring[k] = *(pChar+j);
									j++;
									k++;
								}
								lastCommaPosition = j;
								latitudestring[k] = '\0';

								sscanf(latitudestring, "%f", &latitude);
								latDegrees = (int)(latitude/100);
								latMinutes = (float)(latitude - latDegrees*100);
								
								LCD_WriteString("Lat:");
								LCD_WriteString(latitudestring);
								LCD_WriteString("Lat-Degree:");
								gotoloc(2,1);
								LCD_Write_DATALONG(latDegrees);
								LCD_WriteString("Minutes:");
								LCD_Write_DATALONG(latMinutes);
								soft_delay_ms(3000);
								LCD_Cmd(0x01);
								/* Get lattitude Cardinal direction */
								pChar = mainstring;
								j = lastCommaPosition + 1;
								k = 0;
								while(*(pChar+j) != 0x2C) 
								{
									latitudeCardinalString[k] = *(pChar+j);
									j++;
									k++;
								}
								lastCommaPosition = j;
								latitudeCardinalString[k] = '\0';
								LCD_WriteString("Lat_Cardinal:");
								LCD_WriteString(latitudeCardinalString);
								soft_delay_ms(3000);
								LCD_Cmd(0x01);
					
								
								
															
								/* Get longitude: dddmm.mmmm */
								pChar = mainstring;
								j = lastCommaPosition + 1;
								k = 0;
								while(*(pChar+j) != 0x2C) 
								{
									longitudestring[k] = *(pChar+j);
									j++;
									k++;
								}
								lastCommaPosition = j;
								longitudestring[k] = '\0';

								//sscanf(longitudestring, "%f", &longitude);
								longDegrees = (int)(longitude/100);
								longMinutes = (float)(longitude - longDegrees*100);
								LCD_WriteString("Lon:");
								LCD_WriteString(longitudestring);
								LCD_WriteString("Lon-Degree:");
								LCD_Write_DATALONG(longDegrees);
								gotoloc(2,1);
								LCD_WriteString("Minutes:");
								LCD_Write_DATALONG(longMinutes);
								soft_delay_ms(3000);
								LCD_Cmd(0x01);
								/* Get lattitude Cardinal direction */
								pChar = mainstring;
								j = lastCommaPosition + 1;
								k = 0;
								while(*(pChar+j) != 0x2C) 
								{
									longitudeCardinalString[k] = *(pChar+j);
									j++;
									k++;
								}
								LCD_WriteString("Lon_Cardinal:");
								LCD_WriteString(longitudeCardinalString);
								soft_delay_ms(3000);
								LCD_Cmd(0x01);
							} /* else not a GPGGA sentence */
				} /* otherwise not a $ character... so loop back until one arrives */
		}while(1);
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
	
	//U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit  
	//U1DLM   = 0x00;
	//U1DLL   = 0x61;
	//U1FCR	  = 0X07;//ENABLE FIFO	
	//U1LCR   = 0x03;//to lock mul  and div value

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
