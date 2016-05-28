#include "car_control.h"

u16 yushe_cishu;
/************************************************************************************************
	* 函数名：go_straight
	* 函数作用：控制小车直行
	* 参数：yscs (预设次数) 预设光电传感器输出方波的次数 pwm_x 时分别给左右两个电机输出不同占空比的pwm波
	* 库版本  ：ST3.5.0
*************************************************************************************************/
	void go_straight( u16 pwm_1 , u16 pwm_2 )//直行函数
{
		
		TIM_SetCompare1(TIM4,pwm_1);//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,pwm_2);
		TIM_SetCompare4(TIM4,0);
}
/************************************************************************************************
	* 函数名：turn_to_right
	* 函数作用：控制小车右转
	* 参数：yscs (预设次数) 预设光电传感器输出方波的次数 pwm_x 时分别给左右两个电机输出不同占空比的pwm波
	* 库版本  ：ST3.5.0
*************************************************************************************************/
	void turn_to_right( u16 pwm_1 , u16 pwm_2 ,u16 yanshi)
{
		//yushe_cishu=yscs;
		TIM_SetCompare1(TIM4,0);//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
		TIM_SetCompare2(TIM4,pwm_1);
		TIM_SetCompare3(TIM4,pwm_2);
		TIM_SetCompare4(TIM4,0);
		delay_ms(yanshi);//根据实际情况更改
		TIM_SetCompare1(TIM4,0);//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
	
}
/************************************************************************************************
	* 函数名：turn_to_left
	* 函数作用：控制小车右转
	* 参数：yscs (预设次数) 预设光电传感器输出方波的次数 pwm_x 时分别给左右两个电机输出不同占空比的pwm波
	* 库版本  ：ST3.5.0
*************************************************************************************************/
	void turn_to_left( u16 pwm_1 , u16 pwm_2,u16 yanshi )
{
		//yushe_cishu=yscs;
		TIM_SetCompare1(TIM4,pwm_1);//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,pwm_2);
		delay_ms(yanshi);//根据实际情况更改
		TIM_SetCompare1(TIM4,0);//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
	
}










