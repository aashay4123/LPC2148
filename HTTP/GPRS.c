#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lcd.h"


#define DEFAULT_BUFFER_SIZE	200						/* Define default buffer size */
#define DEFAULT_TIMEOUT		20000					/* Define default timeout */
#define DEFAULT_CRLF_COUNT	2						/* Define default CRLF count */

#define POST				1						/* Define method */
#define GET					0

/* Select Demo */
#define GET_DEMO									/* Define GET demo */
#define POST_DEMO									/* Define POST demo */

/* Define Required fields shown below */
#define URL					"api.thingspeak.com/update"
#define API_WRITE_KEY		"C7JFHZY54GLCJY38"
#define CHANNEL_ID			"119922"
#define APN					"internet"
#define USERNAME			""
#define PASSWORD			""



enum ESP8266_RESPONSE_STATUS							/* Enumerate response status */
{
	ESP8266_RESPONSE_WAITING,
	ESP8266_RESPONSE_FINISHED,
	ESP8266_RESPONSE_TIMEOUT,
	ESP8266_RESPONSE_BUFFER_FULL,
	ESP8266_RESPONSE_STARTING,
	ESP8266_RESPONSE_ERROR
};

char Response_Status, CRLF_COUNT = 0;
uint16_t Counter = 0;
uint32_t TimeOut = 0;
char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];

__irq void UART0_Interrupt(void);

void delay_ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 milisecond delay with Cclk = 60MHz */
	}
}

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;	/* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0x00;	/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 0x61;	/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; /* DLAB = 0 */
	U0IER = 0x00000001; /* Enable RDA interrupts */
}

void UART0_TxChar(char ch) /* A function to send a byte on UART0 */
{
	U0IER = 0x00000000; /* Disable RDA interrupts */
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	/* Wait till THRE bit becomes 1 which tells that transmission is completed */
	U0IER = 0x00000001; /* Enable RDA interrupts */
}

void UART0_SendString(char* str) /* A function to send string on UART0 */
{
	U0IER = 0x00000000; /* Disable RDA interrupts */
	uint8_t i = 0;
	while( str[i] != '\0' )
	{
		UART0_TxChar(str[i]);
		i++;
	}
	U0IER = 0x00000001; /* Enable RDA interrupts */
}

void Read_Response()								/* Read response */
{
	static char CRLF_BUF[2];
	static char CRLF_FOUND;
	uint32_t TimeCount = 0, ResponseBufferLength;
	while(1)
	{
		if(TimeCount >= (DEFAULT_TIMEOUT+TimeOut))	/* Check timecount each time */
		{
			CRLF_COUNT = 0; TimeOut = 0;
			Response_Status = ESP8266_RESPONSE_TIMEOUT;/* if its overflowed then make status timeout and return */
			return;
		}

		if(Response_Status == ESP8266_RESPONSE_STARTING)
		{
			CRLF_FOUND = 0;
			memset(CRLF_BUF, 0, 2);
			Response_Status = ESP8266_RESPONSE_WAITING;/* Make status waiting while reading */
		}
		ResponseBufferLength = strlen(RESPONSE_BUFFER);
		if (ResponseBufferLength)
		{
			delay_ms(1);
			TimeCount++;
			if (ResponseBufferLength==strlen(RESPONSE_BUFFER))
			{
				for (uint16_t i=0;i<ResponseBufferLength;i++)
				{
					memmove(CRLF_BUF, CRLF_BUF + 1, 1);
					CRLF_BUF[1] = RESPONSE_BUFFER[i];
					if(!strncmp(CRLF_BUF, "\r\n", 2))
					{
						if(++CRLF_FOUND == (DEFAULT_CRLF_COUNT+CRLF_COUNT))
						{
							CRLF_COUNT = 0; TimeOut = 0;
							Response_Status = ESP8266_RESPONSE_FINISHED;
							return;
						}
					}
						gotoloc(1,1);
						LCD_WriteString("recieved message");					
						gotoloc(2,1);
						LCD_WriteString(RESPONSE_BUFFER);
				}
				CRLF_FOUND = 0;
			}
		}
		delay_ms(1);
		TimeCount++;
	}
}

void Start_Read_Response()
{
	Response_Status = ESP8266_RESPONSE_STARTING;
	do {
		Read_Response();
	} while(Response_Status == ESP8266_RESPONSE_WAITING);/* Read response till response is waiting */
}

void Buffer_Flush()								/* Flush all variables */
{
	memset(RESPONSE_BUFFER, 0, DEFAULT_BUFFER_SIZE);
	Counter=0;
}

/* Remove CRLF and other default strings from response */ 
void GetResponseBody(char* Response, uint16_t ResponseLength)
{
	uint16_t i = 12;
	char buffer[5];
	while(Response[i] != '\r' && i < 100)
		++i;

	strncpy(buffer, Response + 12, (i - 12));
	ResponseLength = atoi(buffer);

	i += 2;
	uint16_t tmp = strlen(Response) - i;
	memcpy(Response, Response + i, tmp);

	if(!strncmp(Response + tmp - 6, "\r\nOK\r\n", 6))
	memset(Response + tmp - 6, 0, i + 6);
}

bool WaitForExpectedResponse(char* ExpectedResponse)
{
	Buffer_Flush();
	delay_ms(200);
	Start_Read_Response();						/* First read response */
	if((Response_Status != ESP8266_RESPONSE_TIMEOUT) && (strstr(RESPONSE_BUFFER, ExpectedResponse) != NULL))
		return true;							/* Return true for success */
	return false;								/* Else return false */
}

bool SendATandExpectResponse(char* ATCommand, char* ExpectedResponse)
{
	UART0_SendString(ATCommand);				/* Send AT command to esp8266 */
	UART0_TxChar('\r');
	return WaitForExpectedResponse(ExpectedResponse);
}

bool HTTP_Parameter(char* Parameter, char* Value)/* Set HTTP parameter and return response */
{
	
	UART0_SendString("AT+HTTPPARA=\"");
	UART0_SendString(Parameter);
	UART0_SendString("\",\"");
	UART0_SendString(Value);
	UART0_SendString("\"\r");
	return WaitForExpectedResponse("OK");
}

bool ESP8266HTTP_Start()							/* Check esp8266 board */
{
	for (uint8_t i=0;i<5;i++)
	{
		if(SendATandExpectResponse("ATE0","OK")||SendATandExpectResponse("AT","OK"))
		{
			HTTP_Parameter("CID","1");			/* set Bearer profile identifier */
			return true;
		}
	}
	return false;
}
	
bool ESP8266HTTP_Connect() /* change to Esp8266 */
{
	delay_ms(3000); 
	soft_delay_ms(100);
	gotoloc(1,1);	
	UART0_SendString("AT\r\n");                   //Sends AT to ESP8266
	delay_ms(3000);  
	WaitForExpectedResponse("OK");
	Start_Read_Response();
	soft_delay_ms(100);
	UART0_SendString("AT+CWMODE=2\r\n");          //Sends AT+CWMODE=2 (Setting ESP8266 in AP mode)
	delay_ms(3000);
	Start_Read_Response();
	soft_delay_ms(100);
	UART0_SendString("AT+CIFSR\r\n");     	//Sends AT+CIFSR (For getting IP of AP)
	delay_ms(3000);	
	Start_Read_Response();
	soft_delay_ms(100);
	UART0_SendString("AT+CIPMUX=1\r\n");          //Sends AT+CIPMUX=1 (For Mutliple Connections) 
	delay_ms(3000);
	Start_Read_Response();
	soft_delay_ms(100);
	UART0_SendString("AT+CIPSERVER=1,80\r\n");    //Sends AT+CIPSERVER=1,80 (For ENABLING ESP8266 SERVER with OPEN PORT)
	Start_Read_Response();
	delay_ms(3000);
	return WaitForExpectedResponse("OK");

}

bool HTTP_Init()								/* Initiate HTTP */
{
	UART0_SendString("AT+HTTPINIT\r");
	return WaitForExpectedResponse("OK");
}

bool HTTP_Terminate()							/* terminate HTTP */
{
	UART0_SendString("AT+HTTPTERM\r");
	return WaitForExpectedResponse("OK");
}

bool HTTP_SetURL(char * url)					/* Set URL */
{
	return HTTP_Parameter("URL", url);
}

bool HTTP_Connected()							/* Check for connected */
{
	UART0_SendString("AT+SAPBR=2,1\r");
	CRLF_COUNT = 2;										/* Make additional crlf count for response */
	return WaitForExpectedResponse("+SAPBR: 1,1");
}

bool HTTP_SetPost_json()						/* Set Json Application format for post */
{
	return HTTP_Parameter("CONTENT", "application/json");
}

bool HTTP_Save()								/* Save the application context */
{
	UART0_SendString("AT+HTTPSCONT\r");
	return WaitForExpectedResponse("OK");
}

bool HTTP_Data(char* data)						/* Load HTTP data */
{
	char _buffer[25];
	sprintf(_buffer, "AT+HTTPDATA=%d,%d\r", strlen(data), 10000);
	UART0_SendString(_buffer);
	
	if(WaitForExpectedResponse("DOWNLOAD"))
		return SendATandExpectResponse(data, "OK");
	else
		return false;
}

bool HTTP_Action(char method)					/* Select HTTP Action */
{
	if(method == GET)
		UART0_SendString("AT+HTTPACTION=0\r");
	if(method == POST)
		UART0_SendString("AT+HTTPACTION=1\r");
	return WaitForExpectedResponse("OK");
}

bool HTTP_Read(uint8_t StartByte, uint16_t ByteSize) /* Read HTTP response */
{
	char Command[25];
	sprintf(Command,"AT+HTTPREAD=%d,%d\r",StartByte,ByteSize);
	Command[24] = 0;
	UART0_SendString(Command);

	CRLF_COUNT = 2;										/* Make additional crlf count for response */
	if(WaitForExpectedResponse("+HTTPREAD"))
	{
		GetResponseBody(RESPONSE_BUFFER, ByteSize);
		return true;
	}
	else
		return false;
}

uint8_t HTTP_Post(char* Parameters, uint16_t ResponseLength)
{
	HTTP_Parameter("CID","1");			/* set Bearer profile identifier */
	if(!(HTTP_Data(Parameters) && HTTP_Action(POST)))
	return ESP8266_RESPONSE_TIMEOUT;

	bool status200 = WaitForExpectedResponse(",200,");

	if(Response_Status == ESP8266_RESPONSE_TIMEOUT)
	return ESP8266_RESPONSE_TIMEOUT;
	if(!status200)
	return ESP8266_RESPONSE_ERROR;

	HTTP_Read(0, ResponseLength);
	return ESP8266_RESPONSE_FINISHED;
}

uint8_t HTTP_get(char * _URL, uint16_t ResponseLength)
{
	HTTP_Parameter("CID","1");			/* set Bearer profile identifier */
	HTTP_Parameter("URL", _URL);
	HTTP_Action(GET);
	WaitForExpectedResponse("+HTTPACTION:0,");
	if(Response_Status == ESP8266_RESPONSE_TIMEOUT)
	return ESP8266_RESPONSE_TIMEOUT;

	HTTP_Read(0, ResponseLength);
	return ESP8266_RESPONSE_FINISHED;
}

bool esp8266HTTP_Init()
{
	HTTP_Terminate();
	return HTTP_Init();
}

__irq void UART0_Interrupt(void)
{
	RESPONSE_BUFFER[Counter] = U0RBR;				/* Copy data to buffer & increment counter */
	Counter++;
	if(Counter == DEFAULT_BUFFER_SIZE)
		Counter = 0;
	VICVectAddr = 0x00;
}

int main(void)
{
	char _buffer[100];
	
	
	#ifdef POST_DEMO
	uint8_t Sample = 0;
	#endif
	
	VICVectAddr0 = (unsigned) UART0_Interrupt;	/* UART0 ISR Address */
	VICVectCntl0 = 0x00000026;	/* Enable UART0 IRQ slot */
	VICIntEnable = 0x00000040;	/* Enable UART0 interrupt */
	VICIntSelect = 0x00000000;	/* UART0 configured as IRQ */
	UART0_init();
  lcd_init();
	gotoloc(1,1);
	LCD_WriteString("welcome to WIFI");
 	
	while(!ESP8266HTTP_Start());
	while(!(ESP8266HTTP_Connect()));
	esp8266HTTP_Init();

	#ifdef POST_DEMO
	HTTP_SetURL(URL);
	HTTP_Save();
	#endif
	
	while(1)
	{
		if (!HTTP_Connected()) /* Check whether GPRS connected */
		{
		  ESP8266HTTP_Connect();
		  esp8266HTTP_Init();
		}
		
		/* Take local buffer to copy response from server */
		uint16_t responseLength = 100;

		#ifdef POST_DEMO	/* POST Sample data on server */
		memset(_buffer, 0, 100);
		sprintf(_buffer, "api_key=%s&field1=%d", API_WRITE_KEY, Sample++);
		HTTP_Post(_buffer, responseLength);
	  delay_ms(15000);	/* Thingspeak server delay */
		#endif
		
		#ifdef GET_DEMO		/* GET last sample data from server */
		memset(_buffer, 0, 100);
		sprintf(_buffer, "api.thingspeak.com/channels/%s/feeds/last.txt", CHANNEL_ID);
		HTTP_get(_buffer, responseLength);
		#endif
	}
	
}
