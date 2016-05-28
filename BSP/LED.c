/*----------------------------------------------------------------------------
 * �ļ�����:    LED.c
 * ����: LED���ú���Ӧ��API����
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 YunMiao - An Team. All rights reserved.
 *----------------------------------------------------------------------------
 * ���ڣ�2013-08-02
 *----------------------------------------------------------------------------*/

#include "LED.h"

/******************************************************************************
 * ��������: LED_GPIO_Config
 * ��������: ����LED GPIOB 11
 * ��ڲ���: ��
 ******************************************************************************/
void LED_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ʹ��GPIOBʱ��ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/******************************************************************************
 * ��������: LedSet
 * ��������: ���õ������GPIOB 11
 * ��ڲ���: mode :LEDģʽ
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
