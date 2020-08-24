#include<lpc214x.h>
#include "lcd.h"
#include "adc.h"
#include "MQTT.h"
#include "uMQTTBroker.h"



unsigned short adc_value;
 
void process_adc(void)

{
  	unsigned char buf[16] = {0};
	lcd_clear();
	gotoloc(1,1);
  	adc_value = adc_read(ADC0, CHANNEL_1);	
  	sprintf((char *)buf, "ADC VALUE:%d ", adc_value);
  	LCD_WriteString((char *)buf);

}


int main()

{	
	PINSEL1=(1<<19);		// DAC Function Select

	init_adc0();
	lcd_init();
	gotoloc(1,1);
	LCD_WriteString("  Emtron Tech  ");
	gotoloc(2,1);
	LCD_WriteString(" ADC DAC TEST  ");
	soft_delay_ms(3000);
	lcd_clear();

 while(1)
  {
    process_adc();	// Read ADC value and display it on first line of LCD
	DACR=(adc_value<<6);		   // shift data to D6 to D15  of DACR
	soft_delay_ms(200);
  }


}

