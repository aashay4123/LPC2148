#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"

//interrupts
void myGSM(void) __irq;

//function prototypes
void USARTInit(void);
void send_at_commands_gsmgps(void);
void send_msg(void);
void display_rcvd_msg(void);
void display_all_strings(void);//lat,lon,time,mode strings


int main()
{
	lcd_init();
	USARTInit();
	gotoloc(1,1);
	LCD_WriteString("welcome");
	soft_delay_ms(500);
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	send_at_commands_gsmgps();
}
