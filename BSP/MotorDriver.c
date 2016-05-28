/*----------------------------------------------------------------------------
 * 文件名称:    MotorDriver.c
 * 功能: 电机初驱动始化（定时器初始化）
 *----------------------------------------------------------------------------
 *电机驱动使用定时器的PWM红能完成，根据套件使用定时器2完成电机驱动。TM2可以同时
 *输出四路PWM,刚好可以用它作为飞行器四个电极的的驱动信号。引脚PA0(TIM2_CH1),PA1
 *(TIM2_CH2),PA2(TIM2_CH3),PA3(TIM2_CH4)。
 
 * Copyright (c) 2013 YunMiao - An Team. All rights reserved.
 *----------------------------------------------------------------------------
 * 日期：2013-08-02
 *----------------------------------------------------------------------------*/
 
/* 头文件包含 */
#include "MotorDriver.h"


/* -----------------------------------------------------------------------

TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR)
TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR)
TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR)
TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR)
----------------------------------------------------------------------- */
/******************************************************************************
 * 函数名称: TIM2_Config
 * 函数功能: 定时器2初始化
 * 入口参数: 无
 ******************************************************************************/
static void TIM2_Config(void)
{

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  /* 使能定时器2时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999; //定时器计数周期 0-999  1000	
  TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置预分频：不预分频，即为36MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
    //设置跳变值，当计数器计数到这个值时，电平发生跳变(即占空比) 初始值0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    //当定时器计数值小于定时设定值时为高电平
  /* 使能通道1 */
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* 使能通道2 */
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* 使能通道3 */
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
  /* 使能通道4 */
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2重载寄存器ARR
  TIM_Cmd(TIM2, ENABLE); //使能定时器2
}

/******************************************************************************
 * 函数名称: Motor_GPIO
 * 函数功能: 配置电机驱动GPIOA.0.1.2.3
 * 入口参数: 无
 ******************************************************************************/
static void Motor_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 使能GPIOA时钟时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
    GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/******************************************************************************
 * 函数名称: Moto_Init
 * 函数功能: 配置电机驱动GPIOA.0.1.2.3
 * 入口参数: 无
 ******************************************************************************/
void Moto_Init(void)
{
  Motor_GPIO();
  TIM2_Config();
}

/******************************************************************************
 * 函数名称: Motor_Control
 * 函数功能: 输出电机控制信号（PWM）
 * 入口参数: 无
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
