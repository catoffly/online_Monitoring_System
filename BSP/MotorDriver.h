#ifndef _BSP_MOTO_H_
#define _BSP_MOTO_H_

#include "stm32f10x.h"  
#include "system_stm32f10x.h"

#define MAX_CONTROL  999
void Motor_Control(s16 Moter_1, s16 Moter_2, s16 Moter_3, s16
                     Moter_4);
void Moto_Init(void);

#endif
