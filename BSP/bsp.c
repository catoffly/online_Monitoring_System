#include "bsp.h"

void bsp_init(void)
{
  SystemInit(); //系统时钟初始化
	Timer3_init();
	Systick_Init();
	LED_GPIO_Config();//LED引脚初始化
	Moto_Init();	//电机初始化
	I2c_Init();		//IIC初始化
	Delay_ms(50);

}
