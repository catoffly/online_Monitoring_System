/******************** (C) COPYRIGHT 2013 YunMiao ********************
 * File Name          : main.c
 * Author             : YunMiao
 * Version            : V2.0.1
 * Date               : 08/01/20013
 * Description        : IIC basic function
 ********************************************************************************
 ********************************************************************************
 *******************************aircraft****************************************/
#include "i2c.h"


/******************************************************************************
 * 函数名称: I2c_delay
 * 函数功能: I2c 延时函数
 * 入口参数: 无
 ******************************************************************************/
static void I2c_delay(void)
{
    volatile int i = 7;
    while (i)
        i--;
}

/******************************************************************************
 * 函数名称: I2c_Init
 * 函数功能: I2c  GPIO初始化
 * 入口参数: 无
 ******************************************************************************/
 void I2c_Init_MI(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(IIC_GPIO, &GPIO_InitStructure);
}

/******************************************************************************
 * 函数名称: I2c_Start
 * 函数功能: I2c  起始信号
 * 入口参数: 无
 ******************************************************************************/
static uint8_t I2c_Start(void)
{
    SDA_H;
    SCL_H;
    I2c_delay();
    if (!SDA_read)
        return false;
    SDA_L;
    I2c_delay();
    if (SDA_read)
        return false;
    SDA_L;
    I2c_delay();
    return true;
}

/******************************************************************************
 * 函数名称: I2c_Stop
 * 函数功能: I2c  停止信号
 * 入口参数: 无
 ******************************************************************************/
static void I2c_Stop(void)
{
    SCL_L;
    I2c_delay();
    SDA_L;
    I2c_delay();
    SCL_H;
    I2c_delay();
    SDA_H;
    I2c_delay();
}

/******************************************************************************
 * 函数名称: I2c_Ack
 * 函数功能: I2c  产生应答信号
 * 入口参数: 无
 ******************************************************************************/
static void I2c_Ack(void)
{
    SCL_L;
    I2c_delay();
    SDA_L;
    I2c_delay();
    SCL_H;
    I2c_delay();
    SCL_L;
    I2c_delay();
}

/******************************************************************************
 * 函数名称: I2c_NoAck
 * 函数功能: I2c  产生NAck
 * 入口参数: 无
 ******************************************************************************/
static void I2c_NoAck(void)
{
    SCL_L;
    I2c_delay();
    SDA_H;
    I2c_delay();
    SCL_H;
    I2c_delay();
    SCL_L;
    I2c_delay();
}

/*******************************************************************************
 *函数名称:	I2c_WaitAck
 *函数功能:	等待应答信号到来
 *返回值：   1，接收应答失败
 *           0，接收应答成功
 *******************************************************************************/
static uint8_t I2c_WaitAck(void)
{
    SCL_L;
    I2c_delay();
    SDA_H;
    I2c_delay();
    SCL_H;
    I2c_delay();
    if (SDA_read) {
        SCL_L;
        return false;
    }
    SCL_L;
    return true;
}

/******************************************************************************
 * 函数名称: I2c_SendByte
 * 函数功能: I2c  发送一个字节数据
 * 入口参数: byte  发送的数据
 ******************************************************************************/
static void I2c_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--) {
        SCL_L;
        I2c_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2c_delay();
        SCL_H;
        I2c_delay();
    }
    SCL_L;
}

/******************************************************************************
 * 函数名称: I2c_ReadByte
 * 函数功能: I2c  读取一个字节数据
 * 入口参数: 无
 * 返回值	 读取的数据
 ******************************************************************************/
static uint8_t I2c_ReadByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2c_delay();
        SCL_H;
        I2c_delay();
        if (SDA_read) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

/******************************************************************************
 * 函数名称: i2cWriteBuffer
 * 函数功能: I2c       向设备的某一个地址写入固定长度的数据
 * 入口参数: addr,     设备地址
 *           reg，     寄存器地址
 *			 len，     数据长度
 *			 *data	   数据指针
 * 返回值	 1
 ******************************************************************************/
uint8_t i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2c_Start())
        return false;
    I2c_SendByte(addr << 1 | I2C_Direction_Trans);
    if (!I2c_WaitAck()) {
        I2c_Stop();
        return false;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
    for (i = 0; i < len; i++) {
        I2c_SendByte(data[i]);
        if (!I2c_WaitAck()) {
            I2c_Stop();
            return false;
        }
    }
    I2c_Stop();
    return true;
}
/////////////////////////////////////////////////////////////////////////////////
int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	if(i2cWriteBuffer(addr,reg,len,data))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}

/******************************************************************************
 * 函数名称: i2cread
 * 函数功能: I2c  向设备的某一个地址读取固定长度的数据
 * 入口参数: addr,   设备地址
 *           reg，   寄存器地址首地址
 *			 len，   数据长度
 *			 *buf	 数据指针
 * 返回值	 成功 返回 TRUE
 *           错误 返回 FALSE
 ******************************************************************************/
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(i2cRead(addr,reg,len,buf))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}

/*****************************************************************************
 *函数名称:	i2cWrite
 *函数功能:	写入指定设备 指定寄存器一个字节
 *入口参数： addr 目标设备地址
 *		     reg   寄存器地址
 *		     data 读出的数据将要存放的地址
 *******************************************************************************/
uint8_t i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2c_Start())
        return false;
    I2c_SendByte(addr << 1 | I2C_Direction_Trans);
    if (!I2c_WaitAck()) {
        I2c_Stop();
        return false;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
    I2c_SendByte(data);
    I2c_WaitAck();
    I2c_Stop();
    return true;
}

uint8_t i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2c_Start())
        return false;
    I2c_SendByte(addr << 1 | I2C_Direction_Trans);
    if (!I2c_WaitAck()) {
        I2c_Stop();
        return false;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
    I2c_Start();
    I2c_SendByte(addr << 1 | I2C_Direction_Rec);
    I2c_WaitAck();
    while (len) {
        *buf = I2c_ReadByte();
        if (len == 1)
            I2c_NoAck();
        else
            I2c_Ack();
        buf++;
        len--;
    }
    I2c_Stop();
    return true;
}
