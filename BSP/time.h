/* Version            : V2.0.1
* Date               : 08/01/20013
* Description        : IIC basic function
********************************************************************************
********************************************************************************
*******************************aircraft****************************************/

#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f10x.h"
#include "init.h"


/* 供外部调用的函数声明 */
void delay_init(void);
void delay_ms(uint32_t n);
void delay_us(uint32_t n);

uint32_t time_nowMs(void);
uint64_t time_nowUs(void);

void Systick_Init(void);
void Timer3_init(void);
void TIM2_Cap_Init(u16 arr,u16 psc);
void TIM4_Cap_Init(u16 arr,u16 psc);
void TIM1_Cap_Init(u16 arr,u16 psc);
void TIM8_Cap_Init(u16 arr,u16 psc);
void TIM3_Cap_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
extern u8  TIM5CH1_CAPTURE_STA,TIM2CH1_CAPTURE_STA,TIM4CH1_CAPTURE_STA,TIM1CH1_CAPTURE_STA,TIM3CH1_CAPTURE_STA,TIM8CH1_CAPTURE_STA;	//输入捕获状态	//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL,TIM2CH1_CAPTURE_VAL,TIM4CH1_CAPTURE_VAL,TIM1CH1_CAPTURE_VAL,TIM3CH1_CAPTURE_VAL,TIM8CH1_CAPTURE_VAL;	//输入捕获值
#endif 
