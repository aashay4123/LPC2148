#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"


//functions used for GSM
void send_string_0(char *p);//Sends a string of data through UART1
void send_char_0(char data);
unsigned char receive_char_0(void);
void send_at_commands_gsmgps(void);
void send_at(void);
void send_echo(void);
void baud(void);
void send_msg(void);


//FUNCTION PROTOTYPES AND VARIABLES for GPS

//function for GPSs
void get_indian_time(char *a);
void send_pwr_on(void);
void send_rst(void);
void display_all_strings(void);//lat,lon,time,mode strings
void flush(void);
void myGSM(void) __irq;

//variables for gps
static char fullstring[100];
static char time_utc[10];
char mode[2];
char *ptr;
static char lat[8];
static char lon[8];
static unsigned int k_p,l_p,m;

void send_at_commands_gsmgps()
{
	send_at();
	send_echo();
	baud();
	//send_msg();
	send_pwr_on();
	send_rst();
	display_all_strings();
}


void myGSM(void) __irq
{
	unsigned char p;
	p=U0RBR;
	if((p != 0x0d) && (p != 0x0a)&&( p != 'O')&&( p != 'K')&& ( p != 'A') && ( p != 'T'))
	{
		fullstring[k_p++] = p;
	}
  VICVectAddr = 0x00;
}


void send_at()
{
	LCD_WriteString("sending AT");
	soft_delay_ms(200);
	send_string_0("AT");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(100);
	gotoloc(2,1);
}

void send_echo()
{
	gotoloc(2,1);
	soft_delay_ms(200);
	LCD_WriteString("sending echo");
	soft_delay_ms(200);
	send_string_0("ATe0");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(100);
}

//SETTING BAUD RATE TO 9600
void baud()
{	
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		LCD_WriteString("sending BAUD");
		send_string_0("AT+IPR=9600");
		soft_delay_ms(10);
		send_char_0(0x0d);
		soft_delay_ms(100);
				
		//TEXT MODE
		gotoloc(2,1);
		soft_delay_ms(100);		
		LCD_WriteString("sending CMGF");
		send_string_0("AT+CMGF=1");//response is OK
		soft_delay_ms(10);
		send_char_0(0x0d);
		soft_delay_ms(100);
}

//SENDING MESSAGE

/*void send_msg()
{
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		LCD_WriteString("sending CMGS");
		send_string_0("AT+CMGS=\"+918600799520\"");
		send_char_0(0x0d);//carriage return
		soft_delay_ms(2000);
		send_string_0("THIS IS FROM OUR GSM-GPS..");
		send_char_0(0x1a);//ctrlZ
		soft_delay_ms(10);
		send_char_0(0x0d);
		soft_delay_ms(100);
		k_p=0;//since here a charcter reaches the interrupt routine
}		
*/	
		
//ALL FUNCTIONS RELATED TO GPS
void send_pwr_on()
{
	LCD_Cmd(0x01);
	soft_delay_ms(200);
	LCD_WriteString("POWER on");
	soft_delay_ms(200);
	send_string_0("AT+CGPSPWR=1");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(100);
}

void send_rst()
{
	gotoloc(2,1);
	soft_delay_ms(100);
	LCD_WriteString("RESET");
	soft_delay_ms(200);
	send_string_0("AT+CGPSRST=0");
	soft_delay_ms(10);
	send_char_0(0x0d);
	soft_delay_ms(100);
}	

void display_all_strings()
{
		  LCD_Cmd(0x01);
			soft_delay_ms(100);
			LCD_WriteString("INF 2");
			soft_delay_ms(100);
			while(1)
			{	
						LCD_Cmd(0x01);
						soft_delay_ms(300);
						send_string_0("AT+CGPSINF=4");
						soft_delay_ms(5);
						send_char_0(0x0d);
						soft_delay_ms(100);
												
								ptr = fullstring;
								soft_delay_ms(100);
								
								while(*(ptr+l_p) != 0x2C)// for mode
								{
									mode[m] = *(ptr+l_p);
									l_p++;
								}
								mode[1] = '\0';
								l_p++;
								m=0;
								LCD_WriteString("mode:");	
								LCD_WriteString(mode);
								soft_delay_ms(300);
								
															
								LCD_Cmd(0x01);
								soft_delay_ms(100);
								LCD_WriteString("long:");		
								while(*(ptr+l_p) != 0x2C)//for lon
								{
									lon[m++] = *(ptr+l_p);
									l_p++;
								}
								LCD_WriteString(lon);
								soft_delay_ms(500);	
								l_p+=3;
								m=0;
								
								
								LCD_Cmd(0x01);
								soft_delay_ms(250);
								LCD_WriteString("lat:");												
								while(*(ptr+l_p)!= 0x2C)// for lat
								{
									lat[m++] = *(ptr+l_p);
									l_p++;
								}
								LCD_WriteString(lat);
								soft_delay_ms(300);
								m=0;
								l_p+=3;
								
								LCD_Cmd(0x01);
								soft_delay_ms(250);	
								LCD_WriteString("time:");	
								while(*(ptr+l_p) != 0x2C)//for time
								{
									time_utc[m++] = *(ptr+l_p);
									l_p++;
								}
								m=0;
								l_p=0,k_p=0;
								LCD_WriteString(time_utc);
								soft_delay_ms(500);
								gotoloc(2,1);
								soft_delay_ms(100);
								get_indian_time(time_utc);

								
								soft_delay_ms(100);
		}
}	

void get_indian_time(char *t)
{
	char ref[] = {'0','5','3','0','0','0'};
	int c,d;
	c = (int)(t[2] + ref[2]-2*48);
	if(c >= 6)
	{
		c-=6;
		ref[1] += 1;
	}
	t[2] = c+48;
	d = (((int)(t[0]-48)*10 +(int)(t[1]-48))+(int)(ref[1]-48));
	if(d > 23)
		d -= 24;
	t[1] = (char)( d%10 +48);
  t[0] = (char)( d/10 +48);		
	
	LCD_WriteString("h:");
	LCD_Write_DATA(t[0]);
	LCD_Write_DATA(t[1]);
	soft_delay_ms(100);
	LCD_WriteString(" m:");
	LCD_Write_DATA(t[2]);
	LCD_Write_DATA(t[3]);
	soft_delay_ms(100);
	LCD_WriteString(" s:");
	LCD_Write_DATA(t[4]);
	LCD_Write_DATA(t[5]);
	soft_delay_ms(1000);
}

void flush(void)
{
	char dummy;
	while((U0RBR&0x01) == 1)
		dummy = U0RBR;
}
