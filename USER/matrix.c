#include "stm32f10x.h"
#include "matrix.h"
#include "sys.h" 
#include "time.h"

//按键初始化函数
void matrix_KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化 上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTF时钟		|

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;;//PF8~11		行
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF8、9、10、11
	
	GPIO_SetBits(GPIOF,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);//PF8、9、10、11置1
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PA4~7	列
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOF,&GPIO_InitStructure);//初始化GPIOF12\13\14\15
}

int matrix_KEY_Scan(void)
{	
	u8 KeyVal=0;
	u16 temp;

	GPIOF->ODR&=0xf0ff;						//行输出0 0 0 0
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;			//读取列值
	if(temp==0xf000)							//无按下
		return 0;
	else
	{
		delay_ms(5);
		temp=GPIOF->IDR;						
		temp&=0xf000;
		if(temp==0xf000)
		return 0;										//去抖
	}

	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xfeff;						//行输出1 1 1 0
	delay_ms(1);
	temp=(GPIOF->IDR)&=0xf000;		//读列输入
	switch(temp)
	{
		case 0xe000:KeyVal=1;break;
		case 0xd000:KeyVal=2;break;
		case 0xb000:KeyVal=3;break;
		case 0x7000:KeyVal=4;break;
	}
	
	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xfdff;						//行输出1 1 0 1
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;
	switch(temp)
	{
		case 0xe000:KeyVal=5;break;
		case 0xd000:KeyVal=6;break;
		case 0xb000:KeyVal=7;break;
		case 0x7000:KeyVal=8;break;
	}

	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xfbff;					//行输出1 0 1 1
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;
	switch(temp)
	{
		case 0xe000:KeyVal=9;break;
		case 0xd000:KeyVal=10;break;
		case 0xb000:KeyVal=11;break;
		case 0x7000:KeyVal=12;break;
	}

	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xf7ff;					//行输出0 1 1 1
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;
	switch(temp)
	{
		case 0xe000:KeyVal=13;break;
		case 0xd000:KeyVal=14;break;
		case 0xb000:KeyVal=15;break;
		case 0x7000:KeyVal=16;break;
	}

	return KeyVal;							//返回键值
}















