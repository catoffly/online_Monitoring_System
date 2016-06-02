#ifndef _ESP8266_H_
#define _ESP8266_H_
#include "stm32f10x.h"
#include <stdlib.h>
extern char *ATCommandsArray[21];
extern char *ESP_Responses[11];
void esp8266_ap_server_init(void);
extern short data_exist,data_exist1,data_exist2,data_exist3,data_exist4,data_exist5,data_exist6,data_exist7;
extern u8 data_length,data_length1,data_length2,data_length3,data_length4,data_length5,data_ok,data_ok1;
extern char data_wifi[30],data_wifi1[30];
extern int *nextval;
#endif 


