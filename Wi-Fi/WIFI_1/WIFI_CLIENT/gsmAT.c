//####################################################
//LIBRARIES TO BE INCLUDED
//####################################################
#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"


//USER-DEFINED FUNCTIONS
void send_string(char *p);    //Sends a string of data through UART1
void send_char(char data);    //Sends a character of data through UART1
void USARTInit(void);         //Initialize UART1
unsigned char receive_char(void);     
void Checkwifi_Response(void);           //Handle response
void Checkwifi_Response1(void);          //Handle response
void flush(char *s);                        //Flush contents of a sting
void wifi_response(void);                     //Handle response
void wifi_response1(void);                //Handle response
void wifi_response2(void);                //Handle response




//######################################################
//                   Main Code
//######################################################
int main()
{
	  char HELLO[]="Hello!\r\n\r\n";    //String to be sent
	
	//#####################################################
	//              Initializtion functions
	//#####################################################
	  lcd_init();
	  USARTInit();
	
  	gotoloc(1,1);
  	LCD_WriteString("welcome to WIFI");
	  soft_delay_ms(500);
		LCD_Cmd(0x01);
		soft_delay_ms(100);	
	
		 //sending AT command
		 LCD_WriteString("sending AT");
		 soft_delay_ms(1000);
		 send_string("AT");
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
	   send_char(0x0a);
		 wifi_response();
		 soft_delay_ms(100);
  	 soft_delay_ms(1000);
/*		
		//Sending AT command for changing mode 
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		LCD_WriteString("Mode selection");
		 soft_delay_ms(1000);
		 send_string("AT+CWMODE=1");     //AT command for configuring module mode
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response();
		 soft_delay_ms(100);
		 soft_delay_ms(1000);
		 
		 
		 
		 
		 //Sending AT command for multi connection settings
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("Multi connections");
		 soft_delay_ms(1000);
		 send_string("AT+CIPMUX=1");      //AT command for mulltiplle connections
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response();
		 soft_delay_ms(100);
		 soft_delay_ms(1000);
		 
	*/	 
		 
		 
		 //Sending AT command for joining wifi network
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("connecting to");
		 gotoloc(2,1);
		 LCD_WriteString("wifi network");
		 soft_delay_ms(1000);
		 send_string("AT+CWJAP=\"Aashay\",\"aashay123\"");    //AT command to join network
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response();
		 soft_delay_ms(100);
	
	
	
	
	
     /*
		   Sending data
		   1.Initialize TCP connection with server.
			 2.Send data along established connection
			 */
     LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 LCD_WriteString("Sending AT+CIPSTART");
		 soft_delay_ms(10);
		 send_string("AT+CIPSTART=1,\"TCP\",\"172.168.0.1\",1336");    //AT command to start a TCP connection
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response();
		 soft_delay_ms(1000);
		 
		 send_string("AT+CIPSEND=1,8");        //AT command to send data using TCP.
		                                       //Here number of chracters to be transmitted are also passed.
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response();
	   soft_delay_ms(100);
		// soft_delay_ms(1000);
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("Sending Hello!!");
		 send_string(HELLO);
		 wifi_response();
		 soft_delay_ms(1000);
  
}



//Sends a character of data through UART1
void send_char(char data)
{
	while(!(U1LSR&0x20));
	U1THR=data;
}


//Sends a string of data through UART1
void send_string(char *p)            
{
	while(1)
	{
		if(*p=='\0')
		{
			break;
		}
		else
		{
				send_char(*p++);
	  }	
	}		
}



unsigned char receive_char()
{
	while(!(U1LSR&0x01));
	return U1RBR;
}	


//Handle Response
void Checkwifi_Response()
{
	char buff[16],ch;
	int i=0;
	int j=0;	
	while(1)
	   {
			ch= receive_char();	
			buff[i]= ch;
		if((ch==0x0d) && (i!=0))
		  {
				
		   	buff[i]='\0';
				break;
		  }	  
			i++;
	   }
		ch = receive_char();
	  ch = receive_char();
  	LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString(buff);
		soft_delay_ms(500);
		flush(buff);
			/*while(1)
	    {
			ch= receive_char();	
			buff[j]= ch;
			if((ch==0x0d) && (j!=0))
		{
				
		    	buff[j]='\0';
				break;
		}	  
			j++;
	    }*/
		for(j=0;j<8;j++)
			{
				buff[j]=receive_char();
			}
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(2,1);
		LCD_WriteString(buff);
		flush(buff);
		for(j=0;j<8;j++)
		{
				buff[j]=receive_char();
		}
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString(buff);
		flush(buff);
}


//Handle Response
void wifi_response1()
{
  char buff[100],ch;
	int i=0,j=0;
		
	while(1)
	    {
			 ch= receive_char();	
			 buff[i]= ch;
			 if((ch==0x0d) && (i!=0))
		   {
				
	    	buff[i]='\0';
				break;
		   }	  
			 i++;
	    }
	    ch = receive_char();
			ch = receive_char();
			ch = receive_char();
			ch = receive_char();
	
		  soft_delay_ms(100);
   	  LCD_Cmd(0x01);
		  soft_delay_ms(100);
		  gotoloc(1,1);
 	    LCD_WriteString(buff);
				
		  soft_delay_ms(500);
		  //U1FCR=0x02;
		  soft_delay_ms(500);
		  flush(buff);
			while(1)
	    {
			 soft_delay_ms(1000);
			 ch= receive_char();
			 soft_delay_ms(1000);
       //  if(ch>='A' && ch<='Z')				
			 buff[j]= ch;
			 LCD_Cmd(0x01);
			 soft_delay_ms(100);
		   gotoloc(1,1);
			 LCD_Write_DATA(ch);
			 soft_delay_ms(1000);
			 if((ch==0x0d) && (j!=0))
		   {
				
		    	buff[j]='\0';
				  break;
		   }	  
			  j++;
	     }
		  LCD_Cmd(0x01);
		  soft_delay_ms(100);
		  gotoloc(1,1);
		  LCD_WriteString(buff);
	    soft_delay_ms(1000);
		  flush(buff);
			
}



//Handle Response
void wifi_response()
{
  char buff[16],ch;
	int i=0,j=0;
		
	while(1)
	 {
			ch= receive_char();	
			buff[i]= ch;
			if((ch==0x0d) && (i!=0))
	  	{
				
	    	buff[i]='\0';
				break;
		  }	  
			 i++;
	  }
	    ch = receive_char();
			ch = receive_char();
			ch = receive_char();
			ch = receive_char();
	   	LCD_Cmd(0x01);
  		soft_delay_ms(100);
	  	gotoloc(1,1);
		  LCD_WriteString(buff);
			gotoloc(2,1);
			LCD_WriteString("1");
			soft_delay_ms(500);
		  flush(buff);
			while(1)
	    {
			 ch= receive_char();
      //  if(ch>='A' && ch<='Z')				
			 buff[j]= ch;
			
			 if((ch==0x0d) && (j!=0))
		   {
				
		    	buff[j]='\0';
				  break;
		   }	  
			  j++;
	    }
		  LCD_Cmd(0x01);
		  soft_delay_ms(100);
	    gotoloc(1,1);
		  LCD_WriteString(buff);
			gotoloc(2,1);
			LCD_WriteString("2");
		  flush(buff);
}


//Handle Response
void wifi_response2()
{
  char buff[16],ch;
	int i=0,j=0;
		
	    while(1)
	    {
			 ch= receive_char();	
			 buff[i]= ch;
			 if((ch==0x0d) && (i!=0))
   		 {
				
		   	buff[i]='\0';
				break;
		   }	  
			 i++;
	    }
	    ch = receive_char();
			ch = receive_char();
			ch = receive_char();
			ch = receive_char();
	   	LCD_Cmd(0x01);
			soft_delay_ms(100);
		  gotoloc(1,1);
		  LCD_WriteString(buff);	
			gotoloc(2,1);
			LCD_WriteString("3");
			soft_delay_ms(500);
		  flush(buff);
			while(1)
	    {
			ch= receive_char();
    //  if(ch>='A' && ch<='Z')				
			buff[j]= ch;
			
			if((ch==0x0d) && (j!=0))
	   	{
				
		   	buff[j]='\0';
				break;
		  }	  
			j++;
	    }
		  LCD_Cmd(0x01);
			soft_delay_ms(100);
		  gotoloc(1,1);
		  LCD_WriteString(buff);
			gotoloc(2,1);
			LCD_WriteString("4");
		  soft_delay_ms(500);	
		  flush(buff);
			ch = receive_char();
			soft_delay_ms(100);
		
		  i=0;	
		
			while(1)
	    {
			ch= receive_char();	
			buff[i]= ch;
			 if((ch==0x0d) && (i!=0))
		   {
				
		    	buff[i]='\0';
				break;
		   }	  
			 i++;
	    }	
		  ch = receive_char();
			soft_delay_ms(100);
		  LCD_Cmd(0x01);
			soft_delay_ms(100);
		  gotoloc(1,1);
		  LCD_WriteString(buff);
			gotoloc(2,1);
	    LCD_WriteString("5");	
			soft_delay_ms(500);
		  flush(buff);
		
}

//Handle Response
void Checkwifi_Response1()
{
	 char buff[16],ch;
	 int i=0;
	 int j=0;	
	 ch=receive_char();
	 while(1)
	 {
			ch= receive_char();	
			buff[i]= ch;
			if((ch==0x0d) && (i!=0))
		  {
				
		    	buff[i]='\0';
				break;
		  }	  
			i++;
	  }
		//ch = receive_char();
		//ch = receive_char();
  	LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString(buff);
		soft_delay_ms(500);
		flush(buff);
		while(1)
	  {
			ch= receive_char();	
			buff[j]= ch;
			if((ch==0x0a) && (j!=0))
		  {
				
		    	buff[j]='\0';
				  break;
		  }	  
			j++;
	    }	 
		LCD_Cmd(0x01);
		soft_delay_ms(100);
		gotoloc(1,1);
		LCD_WriteString(buff);
		flush(buff);
}		


//Flush string
void flush(char *s)
{
		while(*s!='\0')
		{
		   *s='\0';
    }
}		


//Initialize UART1
void USARTInit()
{
	PINSEL0 = 0x00050005;  //Select TxD for P0.0 and RxD for P0.1
  U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
  U1DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U1FDR=0x00000083;					//MulVal=8 DivVal=3 
	U1FCR	  = 0X07;//ENABLE FIFO	
	U1LCR   = 0x03;//to lock mul  and div value

}


