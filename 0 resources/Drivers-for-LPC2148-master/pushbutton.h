/* Author: Vineeth Kartha
date: 5-10-2013
Description: Header file to handle the push buttons
*/



void initPB()
{
	PINSEL0=(0<<12)|(0<<13)|(0<<15)|(0<<30);
	IO0DIR = (0<<12) | (0<<13)| (0<<15) | (0<<30); //Input Switches	
}
int swdetect(int pinnum)
{
	int portstate=IO0PIN;
	int pinst=(portstate&(1<<pinnum))?0:1; //If PB is not pressed it the pin is in high mode
	return pinst;
}


