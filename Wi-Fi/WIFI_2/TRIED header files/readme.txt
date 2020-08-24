Interfacing ESP8266-01(wifi module) with LPC2148


Configure wifi module as CLIENT and send data to SERVER:
1.Change module to station mode.(AT+CWMODE=1)
2.Enable multiple connections.(AT+CIPMUX=1)
3.Connect to a wifi network/hotspot.(AT+CWJAP)
4.Initiate a TCP connection with the SERVER.(AT+CIPSTART)
  Note:This command should be sent after SERVER is configured.
5.Send data to SERVER.(AT+CIPSEND)


>This process can also be used to upload data to a webserver. 
