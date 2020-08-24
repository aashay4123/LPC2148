#include <lpc214X.h>

#define	EN_PIN_LOC	4
#define	RW_PIN_LOC	3
#define	RS_PIN_LOC	2

#define SET_EN_HI	(IO0SET |= (1UL << EN_PIN_LOC))
#define SET_EN_LO	(IO0CLR |= (1UL << EN_PIN_LOC))

#define SET_RW_HI	(IO0SET |= (1UL << RW_PIN_LOC))
#define SET_RW_LO	(IO0CLR |= (1UL << RW_PIN_LOC))

#define SET_RS_HI	(IO0SET |= (1UL << RS_PIN_LOC))
#define SET_RS_LO	(IO0CLR |= (1UL << RS_PIN_LOC))

//void init(void);
void lcd_init(void);
void delay(int c);
void LCD_Write_DATA(char c);
void LCD_Cmd(unsigned int cmd);
void LCD_WriteString(char * string);
void gotoloc(unsigned char lineno, unsigned char position);
void soft_delay_ms(unsigned int delay);
void soft_delay_us(unsigned int delay_us);

void lcd_init()
{
  PINSEL2 = 0x00000000;
  IO1DIR |= 0x00FF0000;
  IO0DIR |=((1UL << RS_PIN_LOC)|(1UL << RW_PIN_LOC)|(1UL << EN_PIN_LOC));//set p0.2 and p0.3 and p0.4 as an output(enable and register select pins)
  SET_EN_LO;
  SET_RW_LO;
  SET_RS_LO;

  soft_delay_ms(20);
  LCD_Cmd(0x01);
  soft_delay_ms(50);

  LCD_Cmd(0x38); //Function Set Command : 8 Bit Mode , 2 Rows , 5x7 Font Style
  soft_delay_ms(20);

  LCD_Cmd(0x0F); //Display Switch Command : Display on , Cursor on , Blink on
  soft_delay_ms(20);

  LCD_Cmd(0x06); //Input Set : Increment Mode and shift
  soft_delay_ms(20);

  LCD_Cmd(0x0C); //Screen Clear Command , Cursor at Home
  soft_delay_ms(20);
}

void LCD_Write_DATA(char c)
{
	unsigned int port_pin_sts, temp = 0;

	SET_RW_LO;
	SET_RS_HI;
	soft_delay_us(2);
	temp = c;
	temp <<= 16;
	port_pin_sts = IO1PIN;
	port_pin_sts &= 0xFF00FFFF;
	port_pin_sts |=	temp;
    IO1PIN = port_pin_sts; //Supply Character Code
	soft_delay_us(2);
	SET_EN_HI;
	soft_delay_us(2);
	SET_EN_LO;
	soft_delay_us(2);
}

void LCD_WriteString(char * string)
{
    int c=0;
    while (string[c]!='\0')
    {
        LCD_Write_DATA(string[c]);
        c++;
    }
}

void delay(int c)
{
    int i,j;
    for(i=0; i<35; i++)
	{
		for(j=0;j<c;j++);
	}
}       
void LCD_Cmd(unsigned int cmd)
{
	unsigned int port_pin_sts, temp = 0;

	SET_RW_LO;
	SET_RS_LO;
	soft_delay_us(2);
	temp = cmd;
	temp <<= 16;
	port_pin_sts = IO1PIN;
	port_pin_sts &= 0xFF00FFFF;
	port_pin_sts |=	temp;
	IO1PIN = port_pin_sts; //Supply Instruction/Command Code
	soft_delay_us(2);
	SET_EN_HI;
    soft_delay_us(2);
	SET_EN_LO;
	soft_delay_us(2);
}

void gotoloc(unsigned char lineno, unsigned char position)
{
	unsigned char newaddr;

	if(lineno == 1)
		newaddr = 0x80 + (position - 1);
	else if(lineno == 2)
		newaddr = 0xC0 + (position - 1);

	LCD_Cmd(newaddr);	// write DDRAM address according to new cursor location.
}

void soft_delay_ms(unsigned int delay)
{
volatile unsigned int x, y;

for(y = 0; y < delay; y++)
	{
	for(x = 0; x < 7452; x++)
		;
	}
//	for(x = 0; x < 172013; x++);
}

void soft_delay_us(unsigned int delay_us)
{
volatile unsigned int x, y;

for(y = 0; y < delay_us; y++)
	{
	for(x = 0; x < 75; x++)
		;
	}
//	for(x = 0; x < 172013; x++);
}
