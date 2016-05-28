
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include  <stdio.h>
#include  <string.h>
#include "usart1.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "init.h"
#include "time.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
float Yaw_y,Roll_y,Pitch_y;//定义一个姿态预设值
u8 sys_state=0;//重要！！！！！！这是系统状态变量
void diantiao_init(void)//初始化电调
{
	TIM_SetCompare1(TIM5,360);
	TIM_SetCompare3(TIM4,360);
	TIM_SetCompare4(TIM1,360);
	TIM_SetCompare1(TIM1,360);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	TIM_SetCompare1(TIM5,180);
	TIM_SetCompare3(TIM4,180);
	TIM_SetCompare4(TIM1,180);
	TIM_SetCompare1(TIM1,180);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);


}
void RCC_Configuration(void)//外部中断
{
	   GPIO_InitTypeDef GPIO_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		EXTI_InitTypeDef EXTI_InitStructure;
    SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
;
	//KEY
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
// 	EXTI_ClearITPendingBit(EXTI_Line2);
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2); 
// 	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
// 	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
// 	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure);
}
// void EXTI2_IRQHandler(void)
// {
//   
// 		sys_state=1;
//    	EXTI_ClearITPendingBit(EXTI_Line5);


// 		
// }


