#ifndef _ESP8266_H_
#define _ESP8266_H_
#include "stm32f10x.h"
#include <stdlib.h>
extern char *ATCommandsArray[21];
extern char *ESP_Responses[11];
void esp8266_ap_server_init(void);
extern short data_exist,data_exist1,data_exist2,data_exist3;
extern u8 data_length,data_length1,data_length2,data_ok;
extern char data_wifi[30];
extern int *nextval;
#endif 


