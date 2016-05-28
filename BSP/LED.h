#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"  
#include "system_stm32f10x.h"

#define LED1_ON		GPIOE->BRR = GPIO_Pin_6
#define LED1_OFF	GPIOE->BSRR = GPIO_Pin_6

#define LED_MODE_OFF         0x00
#define LED_MODE_ON          0x01
#define LED_MODE_TOGGLE      0x02




void LED_GPIO_Config(void);
void LedSet (u8 mode);

#endif
