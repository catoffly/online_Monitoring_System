#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "time.h"
#include "test.h"
#include  <stdio.h>
#include  <string.h>
#include "usart1.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "init.h"
#include "key_16.h"
#include "matrix.h"
#include "car_control.h"
#include "esp8266.h"
#include "math.h"
#include "crc8.h"


#define ULTRA_PORT			GPIOB
#define ULTRA_CLK       RCC_APB2Periph_GPIOB
#define ULTRA_TRIG			GPIO_Pin_5

#define TRIG_Send  	PBout(5)
unsigned short juli[6];
u8 data_right[30],data_long, data_right1[30];
char data_test=0;
short int  j=5,l=10,h,g;
short int  f=7,d=9;
u8 data_pc[100],torque_long,revolving_long=5;//发送给pc端的数据包，转矩的数据长度，转速的数据长度
char revolving[5];
u32 temp;
long crc_code;
/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//DB0       接PD14 
//DB1       接PD15 
//DB2       接PD0 
//DB3       接PD1 
//DB4~DB12  依次接PE7~PE15
//DB13      接PD8 
//DB14      接PD9
//DB15      接PD10  
//=======================================液晶屏控制线接线==========================================//
//LCD_CS	接PG12	//片选信号
//LCD_RS	接PG0	//寄存器/数据选择信号
//LCD_WR	接PD5	//写信号
//LCD_RD	接PD4	//读信号
//LCD_RST	接PC5	//复位信号
//LCD_LED	接PB0	//背光控制信号(高电平点亮)
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//MO(MISO)	接PF8	//SPI总线输出
//MI(MOSI)	接PF9	//SPI总线输入
//PEN		接PF10	//触摸屏中断信号
//TCS		接PB2	//触摸IC片选
//CLK		接PB1	//SPI总线时钟
//=========================================已使用GPIO、模块=========================================//
tim 1/2/3/4/5/8 全部用为输入捕获
A8/A1/C7/B6/A0/C6 分别对应定时器1到8.
usart1 拟用作与电脑通信方便调试 A9 A10
usart2 拟用作与ESP8266WiFi模块通信 A2 A3
usart3 拟用作与esp8266wfi模块通信 B10 B11
can1   拟用作与工用屏通信 B8 B9
**************************************************************************************************/

extern void MPU6050_Dmp_Init(void);
extern void I2c_Init_MI(void);
extern void Timer3_init(void);
extern void Read_mpu650(void);
extern void control(void);
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;           
 	RCC_APB2PeriphClockCmd(ULTRA_CLK|RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =ULTRA_TRIG;       //发送电平引脚
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 	GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0+GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void Ultra_Ranging(void)//
{
		
	if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{

			juli[0]=TIM5CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM5CH1_CAPTURE_VAL+20>65535)
				juli[0]=65535;
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[0]=65535;
			temp=0;
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
	if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			juli[1]=TIM3CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM3CH1_CAPTURE_VAL+20>65535)
				juli[1]=65535;
			temp=TIM3CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[1]=65535;
			temp=0;
			TIM3CH1_CAPTURE_STA=0;//开启下一次捕获
		}
	if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			juli[2]=TIM2CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM2CH1_CAPTURE_VAL+20>65535)
				juli[2]=65535;
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[2]=65535;
			temp=0;
			TIM2CH1_CAPTURE_STA=0;//开启下一次捕获
		}
	if(TIM4CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			juli[3]=TIM4CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM4CH1_CAPTURE_VAL+20>65535)
				juli[3]=65535;
			temp=TIM4CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[3]=65535;
			temp=0;
			TIM4CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		if(TIM1CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			juli[4]=TIM1CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM1CH1_CAPTURE_VAL+20>65535)
				juli[4]=65535;
			temp=TIM1CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[4]=65535;
			temp=0;
			TIM1CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		if(TIM8CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			juli[5]=TIM8CH1_CAPTURE_VAL+20;//得到总的高电平时间
			if(TIM8CH1_CAPTURE_VAL+20>65535)
				juli[5]=65535;
			temp=TIM8CH1_CAPTURE_STA&0X3F;
			if(temp>0)
				juli[5]=65535;
			temp=0;
			TIM8CH1_CAPTURE_STA=0;//开启下一次捕获
		}

}


int main(void)
{	
	u8 i=0,m=0;
	//char buff[6];
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	USART1_Config();
	delay_init();	     //延时初始化
	Ultran_Init();
	I2c_Init_MI();
	TIM5_Cap_Init(0xfffe,8);
	TIM2_Cap_Init(0xfffe,8);
	TIM3_Cap_Init(0xfffe,8);
	TIM1_Cap_Init(0xfffe,8);
	TIM4_Cap_Init(0xfffe,8);
	TIM8_Cap_Init(0xfffe,8);
	USART1_Config();
	USART2_Config();
	USART3_Config();
	delay_ms(1000);
	delay_ms(1000);
	esp8266_ap_server_init();
	for(i=0;i<99;i++)//数组初始化
	{
		data_pc[i]=0x30;
	}
	data_pc[0]=0xff;
	data_pc[1]=0xff;
  while(1)
{	
//		char *buff[10]={"AT",
//	"AT+CIPSTATUS"};
	Ultra_Ranging();
	if(state_0x0a==1)//字符串解析函数
	{
		u8 i=0,weizhi=0;
		data_exist=bf( data_cu,ESP_Responses[5]);
		if(data_exist!=-1)
		{
			data_exist1=bf( data_cu,ESP_Responses[10]);
			if(data_exist1!=-1)
			{	
				data_exist2=data_exist1-data_exist-7;
				if(data_exist2==1)
				{
					data_length1=data_cu[data_exist+7];
					data_length1=data_length1-0x30;
					data_length=data_length1;
					weizhi=9;
				}

				if(data_exist2==2)
				{
					data_length1=data_cu[data_exist+7];
					data_length1=data_length1-0x30;
					data_length1=data_length1*10;

					data_length2=data_cu[data_exist+8];
					data_length2=data_length2-0x30;
					data_length=data_length1+data_length2;
					weizhi=10;
				}

				for(i=0;i<data_length;i++)
				{
					data_wifi[i]=data_cu[data_exist+weizhi+i];				
				}
//				USART_printf(USART1, data_right);//发送通过校验的数据到电脑上方便调试
//				USART_printf(USART1, "\r\n");
			}


		}
		cun=0;
		for(i=0;i<100;i++)//清空接收数组
		{
			data_cu[i]=0;
		}
		state_0x0a=0;//接收状态变量清零
		//data_exist=0;
	}
	if(state_0x0a1==1)//字符串解析函数
	{
		u8 i=0,weizhi=0;
		data_exist3=bf( data_cu1,ESP_Responses[5]);
		if(data_exist3!=-1)
		{
			data_exist4=bf( data_cu1,ESP_Responses[10]);
			if(data_exist4!=-1)
			{	
				data_exist5=data_exist4-data_exist3-7;
				if(data_exist5==1)
				{
					data_length4=data_cu1[data_exist3+7];
					data_length4=data_length4-0x30;
					data_length3=data_length4;
					weizhi=9;
				}

				if(data_exist5==2)
				{
					data_length4=data_cu1[data_exist3+7];
					data_length4=data_length4-0x30;
					data_length4=data_length4*10;

					data_length5=data_cu1[data_exist3+8];
					data_length5=data_length5-0x30;
					data_length3=data_length4+data_length5;
					weizhi=10;
				}

				for(i=0;i<data_length3;i++)
				{
					data_wifi1[i]=data_cu1[data_exist3+weizhi+i];				
				}
				//USART_printf(USART1, data_right);//发送通过校验的数据到电脑上方便调试
				//USART_printf(USART1, "\r\n");
			}


		}
		cun1=0;
		for(i=0;i<100;i++)//清空接收数组
		{
			data_cu1[i]=0;
		}
		state_0x0a1=0;//接收状态变量清零
		//data_exist=0;
	}	 
	data_test=0;
	for(data_long=0;data_long<9;data_long++)//将接收到的数据进行加和，准备进行校验
	{
		data_test+=data_wifi[data_long];

	}
	if(data_test==data_wifi[9])//校验通过就将数据转移并且保存
	{
		for(data_long=0;data_long<9;data_long++)
		{
			data_right[data_long]=data_wifi[data_long];

		}
	}
	data_test=0;
	for(data_long=0;data_long<9;data_long++)//将接收到的数据进行加和，准备进行校验
	{
		data_test+=data_wifi1[data_long];

	}
	if(data_test==data_wifi1[9])//校验通过就将数据转移并且保存
	{
		for(data_long=0;data_long<9;data_long++)
		{
			data_right1[data_long]=data_wifi1[data_long];

		}
	}
	
	for(i=0;i<6;i++)//将转速数据放入data_pc数据包
	{
		sprintf(revolving,"%d",juli[i]);
		for(m=0;m<5;m++)
		{
			if(revolving[m]==0)
				revolving[m]=0xaa;
		}
		for(m=revolving_long;m>0;m--)
		{
			data_pc[i*revolving_long+m+1]=revolving[m-1];

		}
		for(m=0;m<revolving_long;m++)							//将revolving在这里清零
		{
			revolving[m]=0;
		}
	}

	for(i=0;i<8;i++)
	{
		data_pc[32+i+8*(data_right[8]-1)]=data_right[i];
	}
	for(i=0;i<8;i++)
	{
		data_pc[32+i+8*(data_right1[8]-1)]=data_right1[i];
	}	
      crc_code = yb_crc(data_pc,80);//计算CRC校验码
			
	USART_printf(USART1, data_pc);//发送通过校验的数据到电脑上方便调试
	USART_printf(USART1, "\r\n");
	//printf(data_right);
	//printf("\r\n");
	//			printf("电机1=%d ",juli);
	//			printf("电机2=%d\r\n",juli1);
	//			printf("*********\r\n");
	//USART_printf(USART1,buff[1] );
	//delay_ms(100);
	//esp8266_ap_server_init();
	}
			//printf("电机1=%d\s\n",juli);
			//printf("电机2=%s\r\n",juli1);
			
 }
 


