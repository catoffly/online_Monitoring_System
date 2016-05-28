/*----------------------------------------------------------------------------
 * 文件名称:    LED.c
 * 功能: LED配置和相应的API函数
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 YunMiao - An Team. All rights reserved.
 *----------------------------------------------------------------------------
 * 日期：2013-08-02
 *----------------------------------------------------------------------------*/

#include "LED.h"

/******************************************************************************
 * 函数名称: LED_GPIO_Config
 * 函数功能: 配置LED GPIOB 11
 * 入口参数: 无
 ******************************************************************************/
void LED_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 使能GPIOB时钟时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/******************************************************************************
 * 函数名称: LedSet
 * 函数功能: 配置电机驱动GPIOB 11
 * 入口参数: mode :LED模式
 ******************************************************************************/
void LedSet (u8 mode)
{

	switch(mode)
	{
		case LED_MODE_OFF:
			   	LED1_OFF;
				break;
		case LED_MODE_ON:
		         LED1_ON;
				break;
		case LED_MODE_TOGGLE:
				GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)((1- GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11))));
				break;
		default:
                break;			
	}
	
}
