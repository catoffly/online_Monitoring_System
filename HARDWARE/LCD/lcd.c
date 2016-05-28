#include "lcd.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,主频72M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
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
**************************************************************************************************/	
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


//******************************************************************
//函数名：  LCD_WR_REG
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(u16 data)
{ 
	LCD->LCD_REG=data;//写入要写的寄存器序号	 	
}

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	LCD->LCD_RAM=data;

}
//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
	LCD_WR_DATA(color); 
}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	     		 
}		   
	 
//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//函数名：  LCD_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//函数名：  LCD_Clear
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{	
	u32 index=0; 
	     
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	

	for(index=0;index<153600;index++)
	{
		LCD->LCD_RAM=Color;		  
	}
} 


//******************************************************************
//函数名：  LCD_GPIOInit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    液晶屏IO初始化，FSMC总线初始化，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5复液晶屏位
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x02;	 //0x01 地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x05;		 ////0x03 数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 
}
//******************************************************************
//函数名：  LCD_Reset
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}
 	 
//******************************************************************
//函数名：  LCD_Init
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();

	//************* Start Initial Sequence **********//		
	/*******power on*******/
	LCD_WriteReg(0x15,0x0030);
	LCD_WriteReg(0x9A,0x0010);
	LCD_WriteReg(0x11,0x0020);
	LCD_WriteReg(0x10,0x3428);
	LCD_WriteReg(0x12,0x0002);
	LCD_WriteReg(0x13,0x1046);
	delay_us(40000);//40ms
	LCD_WriteReg(0x12,0x0012);
	delay_us(40000);//40ms
	LCD_WriteReg(0x10,0x3420);
	LCD_WriteReg(0x13,0x3046);
	delay_us(70000);//70ms

	/******gamma setting******/
	LCD_WriteReg(0x30,0x0000);
	LCD_WriteReg(0x31,0x0402);
	LCD_WriteReg(0x32,0x0307);
	LCD_WriteReg(0x33,0x0304);
	LCD_WriteReg(0x34,0x0004);
	LCD_WriteReg(0x35,0x0401);
	LCD_WriteReg(0x36,0x0707);
	LCD_WriteReg(0x37,0x0305);
	LCD_WriteReg(0x38,0x0610);
	LCD_WriteReg(0x39,0x0610);

	/********display mode******/
	LCD_WriteReg(0x01,0x0100);
	LCD_WriteReg(0x02,0x0300);
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x08,0x0808);
	LCD_WriteReg(0x0A,0x0008);
	LCD_WriteReg(0x60,0x2700);
	LCD_WriteReg(0x61,0x0001);

	LCD_WriteReg(0x90,0x013E);
	LCD_WriteReg(0x92,0x0100);
	LCD_WriteReg(0x93,0x0100);
	LCD_WriteReg(0xA0,0x3000);
	LCD_WriteReg(0xA3,0x0010);

	/*******display on*******/
	LCD_WriteReg(0x07,0x0001);
	LCD_WriteReg(0x07,0x0021);
	LCD_WriteReg(0x07,0x0023);
	LCD_WriteReg(0x07,0x0033);
	LCD_WriteReg(0x07,0x0133);
	LCD_WR_REG(0x22);


	LCD_SetParam();//设置LCD参数	 
	LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);
}
  		  
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	if(lcddev.dir==1)//横屏
	{	
	LCD_WR_REG(0x52);	
	LCD_WR_DATA(xStar);
	LCD_WR_REG(0x53);		
	LCD_WR_DATA(xEnd);
	LCD_WR_REG(0x50);	
	LCD_WR_DATA(yStar);
	LCD_WR_REG(0x51);		
	LCD_WR_DATA(yEnd);
	}
	else 
	{
	LCD_WR_REG(0x50);	
	LCD_WR_DATA(xStar);
	LCD_WR_REG(0x51);		
	LCD_WR_DATA(xEnd);
	LCD_WR_REG(0x52);	
	LCD_WR_DATA(yStar);
	LCD_WR_REG(0x53);		
	LCD_WR_DATA(yEnd);
	}
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar);	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos);

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos);

	LCD_WriteRAM_Prepare();	//开始写入GRAM	
} 

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x22;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x21;
	lcddev.setycmd=0x20;
	//第一组参数
	LCD_WriteReg(0x01,0x0100);			
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0xA700);
	//第二组参数，原点在对角线
	/*
	LCD_WriteReg(0x01,0x0000);			
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0x2700);*/

#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x20;
	lcddev.setycmd=0x21;
	/* //第一组参数
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0x2700);*/
	//第二组参数，原点在对角线
	LCD_WriteReg(0x01,0x0000);//add	
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0xA700);
#endif
}	  


