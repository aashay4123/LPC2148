#include<LPC214X.h>
#include "Serial.h"
#include "adc.h"
#include "lcd.h"
#include<String.h>
#include<stdio.h>
#include<stdlib.h>

char api[120];

void generateApi(int adcVal1, int adcVal2)
{ 
	sprintf(api,"GET /update?api_key=51QLHTYXC3MBQD0O&field1=%4d&field2=%4d HTTP/1.0\r\nHost:api.thingspeak.com\r\n\r\n",adcVal1,adcVal2);
}


void flush(char *s){
	
	while(*s){
		*(s++) = '\0';
	}
}

int main(){

	//Initialisation
	lcd_init();
	init_adc0();
	uart1_init();// for esp and ARM 
	uart0_init();// for serial terminal of PC
	
	char adc[4];
	
	//Sending AT
	gotoloc(1,1);
	LCD_WriteString("SENDING AT");
	uart1_puts("AT\r\n");
	soft_delay_ms(1000);
	
	//Setting Mode=1
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	gotoloc(1,1);
	LCD_WriteString("MODE=1");
	uart1_puts("AT+CWMODE=1\r\n");
	soft_delay_ms(1000);
	
	//Connecting to AP
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	gotoloc(1,1);
	LCD_WriteString("CONNECTING TO AP");
	uart1_puts("AT+CWJAP=\"wifi\",\"abcd1234\"\r\n");
	soft_delay_ms(1000);
	soft_delay_ms(500);
	gotoloc(2,1);
	LCD_WriteString("CONNECTED");
	
	//Enabling multiple connections
	LCD_Cmd(0x01);
	soft_delay_ms(100);
	uart1_puts("AT+CIPMUX=1\r\n");
	soft_delay_ms(1000);
	
	
	//variable declaration 
	unsigned short val1,val2;
	
	while(1)// continuously runs this bloock
		{

		//Starting Connection
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString("CONNECTING");
		uart1_puts("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");// if it's UDP then you will not see string in browser but you will see a graph
		soft_delay_ms(3000);
		gotoloc(2,1);
		LCD_WriteString("CONNECTED");

		//Reading ADC value and generating api
		val1 = adc_read(ADC0,CHANNEL_1);//channel 1
		val2 = adc_read(ADC0,CHANNEL_2);//channel 2
		sprintf(adc,"%3d",val1);
		generateApi(val1,val2);
		
		//Send command
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString("SENDING API");
		uart1_puts("AT+CIPSEND=98\r\n");
		soft_delay_ms(300);
		
		//Sending API
		uart1_puts(api);
		uart0_puts(api);
		soft_delay_ms(3000);
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString("SENT");
		soft_delay_ms(100);
		gotoloc(2,1);
		LCD_WriteString(adc);
		soft_delay_ms(1000);
		flush(api);
		
		//totoal 15 sec delay is required
		//7soft_delay_ms(4000);
	}
	
return 0;	
}


