/*----------------------------------------------------------------------------
 * �ļ�����:    MotorDriver.c
 * ����: ���������ʼ������ʱ����ʼ����
 *----------------------------------------------------------------------------
 *�������ʹ�ö�ʱ����PWM������ɣ������׼�ʹ�ö�ʱ��2��ɵ��������TM2����ͬʱ
 *�����·PWM,�պÿ���������Ϊ�������ĸ��缫�ĵ������źš�����PA0(TIM2_CH1),PA1
 *(TIM2_CH2),PA2(TIM2_CH3),PA3(TIM2_CH4)��
 
 * Copyright (c) 2013 YunMiao - An Team. All rights reserved.
 *----------------------------------------------------------------------------
 * ���ڣ�2013-08-02
 *----------------------------------------------------------------------------*/
 
/* ͷ�ļ����� */
#include "MotorDriver.h"


/* -----------------------------------------------------------------------

TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR)
TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR)
TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR)
TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR)
----------------------------------------------------------------------- */
/******************************************************************************
 * ��������: TIM2_Config
 * ��������: ��ʱ��2��ʼ��
 * ��ڲ���: ��
 ******************************************************************************/
static void TIM2_Config(void)
{

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  /* ʹ�ܶ�ʱ��2ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999; //��ʱ���������� 0-999  1000	
  TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
    //��������ֵ�������������������ֵʱ����ƽ��������(��ռ�ձ�) ��ʼֵ0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    //����ʱ������ֵС�ڶ�ʱ�趨ֵʱΪ�ߵ�ƽ
  /* ʹ��ͨ��1 */
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* ʹ��ͨ��2 */
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* ʹ��ͨ��3 */
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* ʹ��ͨ��4 */
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ��TIM2���ؼĴ���ARR
  TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
}

/******************************************************************************
 * ��������: Motor_GPIO
 * ��������: ���õ������GPIOA.0.1.2.3
 * ��ڲ���: ��
 ******************************************************************************/
static void Motor_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ʹ��GPIOAʱ��ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
    GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/******************************************************************************
 * ��������: Moto_Init
 * ��������: ���õ������GPIOA.0.1.2.3
 * ��ڲ���: ��
 ******************************************************************************/
void Moto_Init(void)
{
  Motor_GPIO();
  TIM2_Config();
}

/******************************************************************************
 * ��������: Motor_Control
 * ��������: �����������źţ�PWM��
 * ��ڲ���: ��
 ******************************************************************************/
void Motor_Control(s16 Moter_1, s16 Moter_2, s16 Moter_3, s16
                   Moter_4)
{
  if(Moter_1 > MAX_CONTROL)
  {
    Moter_1 = MAX_CONTROL;
  }
  if(Moter_1 < 0)
  {
    Moter_1 = 0;
  }

  if(Moter_2 > MAX_CONTROL)
  {
    Moter_2 = MAX_CONTROL;
  }
  if(Moter_2 < 0)
  {
    Moter_2 = 0;
  }

  if(Moter_3 > MAX_CONTROL)
  {
    Moter_3 = MAX_CONTROL;
  }
  if(Moter_3 < 0)
  {
    Moter_3 = 0;
  }
  if(Moter_4 > MAX_CONTROL)
  {
    Moter_4 = MAX_CONTROL;
  }
  if(Moter_4 < 0)
  {
    Moter_4 = 0;
  }

  TIM2->CCR1 = Moter_1;
  TIM2->CCR2 = Moter_2;
  TIM2->CCR3 = Moter_3;
  TIM2->CCR4 = Moter_4;
}
