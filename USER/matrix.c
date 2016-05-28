#include "stm32f10x.h"
#include "matrix.h"
#include "sys.h" 
#include "time.h"

//������ʼ������
void matrix_KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ�� ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTFʱ��		|

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;;//PF8~11		��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF8��9��10��11
	
	GPIO_SetBits(GPIOF,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);//PF8��9��10��11��1
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PA4~7	��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOF,&GPIO_InitStructure);//��ʼ��GPIOF12\13\14\15
}

int matrix_KEY_Scan(void)
{	
	u8 KeyVal=0;
	u16 temp;

	GPIOF->ODR&=0xf0ff;						//�����0 0 0 0
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;			//��ȡ��ֵ
	if(temp==0xf000)							//�ް���
		return 0;
	else
	{
		delay_ms(5);
		temp=GPIOF->IDR;						
		temp&=0xf000;
		if(temp==0xf000)
		return 0;										//ȥ��
	}

	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xfeff;						//�����1 1 1 0
	delay_ms(1);
	temp=(GPIOF->IDR)&=0xf000;		//��������
	switch(temp)
	{
		case 0xe000:KeyVal=1;break;
		case 0xd000:KeyVal=2;break;
		case 0xb000:KeyVal=3;break;
		case 0x7000:KeyVal=4;break;
	}
	
	GPIOF->ODR|=0x0f00;
	GPIOF->ODR&=0xfdff;						//�����1 1 0 1
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
	GPIOF->ODR&=0xfbff;					//�����1 0 1 1
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
	GPIOF->ODR&=0xf7ff;					//�����0 1 1 1
	delay_ms(1);
	temp=(GPIOF->IDR)&0xf000;
	switch(temp)
	{
		case 0xe000:KeyVal=13;break;
		case 0xd000:KeyVal=14;break;
		case 0xb000:KeyVal=15;break;
		case 0x7000:KeyVal=16;break;
	}

	return KeyVal;							//���ؼ�ֵ
}















