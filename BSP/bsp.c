#include "bsp.h"

void bsp_init(void)
{
  SystemInit(); //ϵͳʱ�ӳ�ʼ��
	Timer3_init();
	Systick_Init();
	LED_GPIO_Config();//LED���ų�ʼ��
	Moto_Init();	//�����ʼ��
	I2c_Init();		//IIC��ʼ��
	Delay_ms(50);

}
