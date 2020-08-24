#include<LPC214X.h>
#include "Serial.h"
#include "adc.h"
#include "lcd.h"
#include<String.h>
#include<stdio.h>
#include<stdlib.h>


int main(){

	lcd_init();
	LCD_WriteString("asdf");
	
	
	return 0;
}


