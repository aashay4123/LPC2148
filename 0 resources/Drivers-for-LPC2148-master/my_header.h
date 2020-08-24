/* Author: Vineeth Kartha
File: my_header.h
A collection of functions to be used with the LPC2148 board*/


/* Some definitions */
#define Fosc            12000000                    
#define Fcclk           (Fosc * 5)                  
#define Fcco            (Fcclk * 4)                 
#define Fpclk           (Fcclk / 4) * 1

/* A SIMPLE DELAY FUNCTION*/
void delay(unsigned int d)
{
int i=0;
for(;d>0;d--)
for(i=0;i<60000;i++);
}

char * DECtoASCII(unsigned int Data)		 //This function converts decimal data into ASCII
{ 
char Temp[5];
 Temp[3]='C';
 Temp[2]=(char)223;
 Temp[1]=(Data % 10) + 48;
 Temp[0]=(Data / 10) + 48;
 Temp[4]='\0';
return Temp;
}
