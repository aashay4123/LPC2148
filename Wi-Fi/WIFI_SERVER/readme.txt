Interfacing ESP8266-01(wifi module) with LPC2148

Configure wifi module as SERVER:
1.Change module to station mode.(AT+CWMODE=1)
2.Enable multiple connections.(AT+CIPMUX=1)
3.Connect to a wifi network/hotspot.(AT+CWJAP)
4.Setup wifi module as server.(AT+CIPSERVER)
5.Check IP address.(AT+CIFSR)
  Note:This step is used to check whether the module is correctly configured.


>For accepting data from a client, a notification is received by the module.This can be received by the module.Another  way is to use a receiver interrupt(LPC2148's UART receiver interrupt)  