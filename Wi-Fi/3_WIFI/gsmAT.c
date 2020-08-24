//##############################################
//         LIBRARIES INCLUDED
//############################################## 
#include<lpc214x.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "lcd.h"


//#################################################
//          USER DEFINED FUNCTIONS
//#################################################
void send_string(char *p);      //Sends a string of data through UART1
void send_char(char data);      //Sending a character using UART1
void send_char0(char data);     //Sending a character using UART0  
void send_string0(char *p);    //Sends a string of data through UART0
void USARTInit(void);      //Initialize UART0 and UART1
unsigned char receive_char(void);   //Receive a character using UART1
void Checkwifi_Response(void);      //Handle Response
void Checkwifi_Response1(void);     //Handle Response
void flush(char *s);        //Flush a string
void wifi_response(void);           //Handle Response
void wifi_response1(void);          //Handle Response
void wifi_response2(void);          //Handle Response
void wifi_response3(void);          //Handle Response
void wifi_response4(void);          //Handle Response



//########################################
//             MAIN CODE
//########################################
int main()
{
	
	//**************************************
	//       LOCAL VARIABLES
	//**************************************
	char GET[60]="GET /update?key=1UUF7247WILX8JVK&field1=1 HTTP/1.1\r\n"; 
	//char header[20]="GET HTTP/1.1\r\n\r\n";
	char HOST[40]="Host: api.thingspeak.com\r\n\r\n\r\n";
  int count_char=0,int_ch;
	char counta[3],ch;
	 //char data[6]="1234";
   //	int count_char1=0;
	
	
	//#####################################
	//   INITIALIZATION FUNCTIONS
	//#####################################
	
	  lcd_init();
	  USARTInit();
  



	   gotoloc(1,1);
	   LCD_WriteString("welcome to WIFI");
	   soft_delay_ms(500);
  	 LCD_Cmd(0x01);
		 soft_delay_ms(100);
			
		
		 //sending AT command
		 LCD_WriteString("sending AT");
		 soft_delay_ms(500);
		 send_string("AT");
		 soft_delay_ms(10);
		 send_char(0x0d);
	   soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response3();
		 soft_delay_ms(500);
		 
		 
		 
		 //Sending AT command to change mode of operation 
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("Mode selection");
		 soft_delay_ms(500);
		 send_string("AT+CWMODE=1");        //AT command to change mode
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response3();
		 soft_delay_ms(500);
		 
		 
		 
		 //Sending AT command for multiple connections
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("Multi connections");
		 soft_delay_ms(500);
		 send_string("AT+CIPMUX=1");       //AT command for multiple connections
 		 soft_delay_ms(10);
	   send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response3();
		 soft_delay_ms(500);
		 
		 
		 //Sending AT command to connect to a network
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 gotoloc(1,1);
		 LCD_WriteString("connecting to");
		 gotoloc(2,1);
		 LCD_WriteString("wifi network");
		 soft_delay_ms(1000);
		 send_string("AT+CWJAP=\"Windows Phone\",\"1234567890\"");
		 soft_delay_ms(10);
		 send_char(0x0d);
	   soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response3();
		 soft_delay_ms(100);
		 
		 
		 
		 //Start a connection
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
		 count_char=strlen(GET)+strlen(HOST)-2;
		 counta[1]=count_char%10;
		 counta[0]=count_char/10;
		 LCD_WriteString("Sending AT+CIPSTART");
		 soft_delay_ms(10);
		 send_string("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80");
		 soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);
		 wifi_response3();
		 soft_delay_ms(100);
		 LCD_Cmd(0x01);
		 soft_delay_ms(100);
	
		 
		 //Sending data
		 LCD_Cmd(0x01);
		 send_string("AT+CIPSEND=4,");
		 soft_delay_ms(10);
		 send_string(counta);
		 /*soft_delay_ms(10);
		 send_char(0x0d);
		 soft_delay_ms(10);
		 send_char(0x0a);*/
		
			//soft_delay_ms(10);
			//wifi_response1();
			soft_delay_ms(10);
      send_string(GET);
			soft_delay_ms(10);
			send_string(HOST);
      soft_delay_ms(10);
		
		  LCD_WriteString("Sending URL");
			soft_delay_ms(500);
			//wifi_response4();
			for(int_ch=0;int_ch<22;int_ch++)
			{
			  ch=receive_char();
				send_char0(ch);
			}

			
			soft_delay_ms(100);
			LCD_Cmd(0x01);
			soft_delay_ms(10);
			LCD_WriteString("Data Sent");
			send_string("AT+CIPCLOSE=4");
			soft_delay_ms(10);
      send_char(0x0d);
		  soft_delay_ms(10);
			send_char(0x0a);
			soft_delay_ms(10);
			wifi_response4();
			
			
}



//Sends character using UART1
void send_char(char data)
{
	while(!(U1LSR&0x20));
	U1THR=data;
}



//Sends character using UART0
void send_char0(char data)
{
	while(!(U0LSR&0x20));
	U0THR=data;
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


//Sends a string of data through UART0
void send_string0(char *p)        
{
	while(1)
	{
		if(*p=='\0')
		{
			break;
		}
		else
		{
				send_char0(*p++);
	  }	
	}
		
}


//Receive a string using UART1
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
	int i=0;
  while(1)
	{
		ch=receive_char();
		buff[i]=ch;
		if(ch==0x0d && i!=0)
		{
			if(receive_char()==0x0a)
			{
				buff[i]='\0';
				break;
			}
		}
		else
		{
			i++;
		}
	}		send_string0(buff);
		  soft_delay_ms(10);
		  send_char0(0x0d);
		  soft_delay_ms(10);
		  send_char0(0x0a);
	    flush(buff);
	    //soft_delay_ms(10);	
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
void wifi_response4()
{
	char buff[64],ch;
	int i=0;
	int count=0;
	while(1)
	{
		ch=receive_char();
		buff[i]=ch;
		if(ch==0x0d && i!=0)
		{
			count++;
			if(count==2 && receive_char()==0x0a)
			{
				buff[i]='\0';
				break;
			}
		}
		else
		{
			i++;
		}
	}
			send_string0(buff);
		  soft_delay_ms(10);
		  send_char0(0x0d);
		  soft_delay_ms(10);
		  send_char0(0x0a);
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
void wifi_response3()
{
	char buff[128];
	int i=0,count=0,j=0;
	char ch;
	//char ch=receive_char();
	while(1)
	{
		ch=receive_char();
		buff[i]=ch;
		if(ch==0x0d && i!=0)
		{
			count++;
			if(count==2 && receive_char()==0x0a)
			{
				buff[i]='\0';
				break;
			}
		}
		else
		{
			i++;
		}
	}
	 send_string0(buff);
	 soft_delay_ms(10);
	 flush(buff);
	 soft_delay_ms(10);
	 while(1)
	 {
		ch=receive_char();
		buff[j]=ch;
		if(ch==0x0d && j!=0)
		{
			if(receive_char()==0x0a)
			{
				buff[j]='\0';
				break;
			}
		}
		else
		{
			j++;
		}
	 }
	 send_string0(buff);
	 soft_delay_ms(10);
	 send_char0(0x0d);
	 soft_delay_ms(10);
	 send_char0(0x0a);
	 soft_delay_ms(10);
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



//Flush a string
void flush(char *s)
{
	  while(*s!='\0')
		{
		   *s++='\0';
		}
}		


//Intilizes UART0 and UART1
void USARTInit()
{
	PINSEL0 = 0x00050005;  //Select TxD for P0.0 and RxD for P0.1
  U1LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
  U1DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U1FDR=0x00000083;					//MulVal=8 DivVal=3 
	U1FCR	  = 0X07;//ENABLE FIFO	
	U1LCR   = 0x03;//to lock mul  and div value
  U0LCR   = 0x83;// 8 bits, no Parity, 1 Stop bit 
  U0DLL = 6;                     /* 115200 Baud Rate @ 15MHz VPB Clock         */
  U0FDR=0x00000083;					//MulVal=8 DivVal=3 
	U0FCR	  = 0X07;//ENABLE FIFO	
	U0LCR   = 0x03;//to lock mul  and div value

}
