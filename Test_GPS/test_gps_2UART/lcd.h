//void init(void);
void lcd_init(void);
void delay(int c);
void LCD_Write_DATACHAR(char c);
void LCD_Write_DATALONG(char c);
void LCD_Cmd(unsigned int cmd);
void LCD_WriteString(unsigned char * string);
void gotoloc(unsigned char lineno, unsigned char position);
void soft_delay_ms(unsigned int delay);
void soft_delay_us(unsigned int delay_us);

