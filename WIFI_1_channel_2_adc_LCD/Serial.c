#include <LPC214x.H>                     /* LPC21xx definitions               */
#include <stdio.h>
#include "Serial.h"
#include <string.h>

#define CR     0x0D


/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */

  if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;                          /* output CR */
  }
  while (!(U1LSR & 0x20));
  return (U1THR = ch);
}


int uart0_getkey (void)  {                     /* Read character from Serial Port   */

  while (!(U0LSR & 0x01));

  return (U0RBR);
}

int uart1_getkey ()  {                     /* Read character from Serial Port   */

  while (!(U1LSR & 0x01));

  return (U1RBR);
}

void uart1_init()
{
  /* initialize the serial interface   
  Baud= Pclk * mulVal
  		-----------------
		16*(256*U0BLM+U0DLL)*(mulVal+divVal)
   */

 PINSEL0 = 0x00050005;  //Select TxD for P0.0 and RxD for P0.1
  U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
  U1DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U1FDR=0x00000083;					//MulVal=8 DivVal=3 
	U1FCR	  = 0X07;//ENABLE FIFO	
	U1LCR   = 0x03;//to lock mul  and div value                             
}

//------------------------------------------------------------------------------------------------//
//---------------------------- Function for send character 1 time via UART1-----------------------//
//------------------------------------------------------------------------------------------------//
void uart1_putc(char c)
{
	while(!(U1LSR & 0x20)); // Wait until UART1 ready to send character     
	U1THR = c;  // Send character
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function for send string via UART1---------------------------------//
//------------------------------------------------------------------------------------------------//
void uart1_puts(char *p)
{
	
	while(*p) // Point to character
	{
		uart1_putc(*p++);   // Send character then point to next character
	}
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function for Initial UART0 ----------------------------------------//
//------------------------------------------------------------------------------------------------//
void uart0_init()
{
  /* initialize the serial interface   
  Baud= Pclk * mulVal
  		-----------------
		16*(256*U0BLM+U0DLL)*(mulVal+divVal)
   */

  PINSEL0 |= 0x00050005;           /* Enable RxD0 and TxD0                     */
  U0LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
  U0DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U0FDR=0x00000083;					//MulVal=8 DivVal=3 
	U0FCR	  = 0X07;//ENABLE FIFO	
	U0LCR   = 0x03;//to lock mul  and div value
              /* DLAB = 0                                 */
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function for send character 1 time via UART0-----------------------//
//------------------------------------------------------------------------------------------------//
void uart0_putc(char c)
{
	while(!(U0LSR & 0x20)); // Wait until UART0 ready to send character  Transmitter  Holding Register Empty
	U0THR = c; // Send character
}
//------------------------------------------------------------------------------------------------//
//---------------------------- Function for send string via UART1---------------------------------//
//------------------------------------------------------------------------------------------------//
void uart0_puts(char *p)
{
	while(*p) // Point to character
	{
		uart0_putc(*p++);  // Send character then point to next character
	}
}

void uart0_dump(char *p,unsigned short len,unsigned char mod)
{
	unsigned char temp[3];
	unsigned short i=0;
	for(;i<len;i++){
		if(i%mod==0)
			uart0_puts("\n\r");
	 sprintf((char *)temp,"%02x ",p[i]);
	 uart0_puts((char *)temp);
	}
}
