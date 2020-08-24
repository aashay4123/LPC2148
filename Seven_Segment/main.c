#include <LPC214X.h>


void wait(int count)
{
  int j=0,i=0;
 
  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}


int main()
{
	unsigned long i=0;
	  IO1DIR |= 0x00FF0000;							// PORT1.16 to PORT1.23 = A,B,C,D,E,F,G,DP
    IO0DIR|=((unsigned long)1<<4);		// PORT0.2 = Display3 Eanble, PORT0.3 = Display2 Eanble, PORT0.4 = Display1 Eanble,
 	while(1){
 		IO0SET=((unsigned long)1<<4);			//PORT0.4 = Display1 Eanble,
		IO1PIN |= 0x004F0000;							// Display BCD  "3"
		
		wait(100000);
		i++;
	}
}

