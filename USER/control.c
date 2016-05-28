#include"time.h"
#include<math.h>

extern u16 speed1;
extern u16 speed2;
extern u16 speed3;
extern u16 flag_sp;
extern u16 flag_sp_roll;

extern float j_Pitch,j_Roll;

extern  float Yaw,Roll,Pitch;

float target_e=0;			//这个改为25或者更高
float target_e_Roll=0;	//Roll方向的值 

float Ek=0,Ek1=0,Ek2=0;
float Kp=2,Ki=0,Kd=0;		//Kp改成10

float Ek_roll,Kp_roll=1,m_uk_roll=0;
float uk_M3=185,uk_M2=185;

float uk_M4=185,uk_M1=185,m_uk=0,m_uk_ShowLED=0;
float SUM_Ki=0;

float sp=0,sp_optive=0,sp_nagtive=0;	
u16 PWM_M1,PWM_M2;





void control(void)
{
	
			int j,k;
			int l,m;
		
			Ek=target_e-abs(2*tan(j_Pitch)*100);		//获取偏差
		
		//获取PID增量参数
//		m_uk+=Kp*(Ek-Ek1);							//比例部分
		m_uk=Kp*Ek;

 		SUM_Ki+=Ki*Ek;								//积分部分
// 		m_uk+=Kd*(Ek-2*Ek1+Ek2)/0.01;		//微分部分
// 		
// 		
// 		//偏差移位
// 		Ek2=Ek1;				//把前一次偏差赋值到前二次偏差
// 		Ek1=Ek;					//把本次偏差赋值到前一次偏差
	
// 		uk_M4=250+m_uk;						//增量参数加到 M4 电机出上		M4为右方向给力
// 		uk_M1=250-m_uk;						//增量参数加到 M1 电机出上		M1为左方向给力
	//	m_uk_ShowLED=m_uk;	//增量偏差打印到LED上
//		m_uk=0;							//速度增加量清0
	
		if(uk_M4>360)
			uk_M4=360;
		if(uk_M1>360)
			uk_M1=360;		
	
		//输出限幅
		if( uk_M4<190 )	
				uk_M4=190;

		if( uk_M1<190 )
				uk_M1=190;
		
		
			j=190;
			if(flag_sp==0)
			{
				if( ( -5<Pitch) && (Pitch<5 ) )
				{
					j=210+m_uk+SUM_Ki;
					if(j<190)
						j=190;
					if(j>360)
						j=360;
					
				}
			}
			
			
			k=190;
			if(flag_sp==1)
			{
				if( ( -5<Pitch) && (Pitch<5 ) )
				{
					k=200+m_uk+SUM_Ki;
					if(k<190)
						k=190;
					if(k>360)
						k=360;
					
				}
			}
  	  TIM_SetCompare1(TIM1,j);	
			TIM_SetCompare3(TIM4,k);
	
			PWM_M1=j;
			
			
			
			
			Ek_roll=target_e_Roll-abs(2*tan(j_Roll)*100);		//获取偏差			
			m_uk_roll=Kp_roll*Ek_roll;				//roll的比例计算
			
			l=200;
			if(flag_sp_roll==0)
			{
				if( ( -5<Roll) && (Roll<5 ) )
				{
					l=200+m_uk_roll;
					if(l<200)
						l=200;
					if(l>360)
						l=360;
					
				}
			}
			
			m=200;
			if(flag_sp_roll==1)
			{
				if( ( -5<Roll) && (Roll<5 ) )
				{
					m=200+m_uk_roll;
					if(m<200)
						m=200;
					if(m>360)
						m=360;
					
				}
			}
			
		  TIM_SetCompare2(TIM2,l);	
			TIM_SetCompare1(TIM5,m);
			
			PWM_M2=l;
			
			
		


 }
