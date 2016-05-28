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
 * ��������: I2c_delay
 * ��������: I2c ��ʱ����
 * ��ڲ���: ��
 ******************************************************************************/
static void I2c_delay(void)
{
    volatile int i = 7;
    while (i)
        i--;
}

/******************************************************************************
 * ��������: I2c_Init
 * ��������: I2c  GPIO��ʼ��
 * ��ڲ���: ��
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
 * ��������: I2c_Start
 * ��������: I2c  ��ʼ�ź�
 * ��ڲ���: ��
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
 * ��������: I2c_Stop
 * ��������: I2c  ֹͣ�ź�
 * ��ڲ���: ��
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
 * ��������: I2c_Ack
 * ��������: I2c  ����Ӧ���ź�
 * ��ڲ���: ��
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
 * ��������: I2c_NoAck
 * ��������: I2c  ����NAck
 * ��ڲ���: ��
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
 *��������:	I2c_WaitAck
 *��������:	�ȴ�Ӧ���źŵ���
 *����ֵ��   1������Ӧ��ʧ��
 *           0������Ӧ��ɹ�
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
 * ��������: I2c_SendByte
 * ��������: I2c  ����һ���ֽ�����
 * ��ڲ���: byte  ���͵�����
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
 * ��������: I2c_ReadByte
 * ��������: I2c  ��ȡһ���ֽ�����
 * ��ڲ���: ��
 * ����ֵ	 ��ȡ������
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
 * ��������: i2cWriteBuffer
 * ��������: I2c       ���豸��ĳһ����ַд��̶����ȵ�����
 * ��ڲ���: addr,     �豸��ַ
 *           reg��     �Ĵ�����ַ
 *			 len��     ���ݳ���
 *			 *data	   ����ָ��
 * ����ֵ	 1
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
 * ��������: i2cread
 * ��������: I2c  ���豸��ĳһ����ַ��ȡ�̶����ȵ�����
 * ��ڲ���: addr,   �豸��ַ
 *           reg��   �Ĵ�����ַ�׵�ַ
 *			 len��   ���ݳ���
 *			 *buf	 ����ָ��
 * ����ֵ	 �ɹ� ���� TRUE
 *           ���� ���� FALSE
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
 *��������:	i2cWrite
 *��������:	д��ָ���豸 ָ���Ĵ���һ���ֽ�
 *��ڲ����� addr Ŀ���豸��ַ
 *		     reg   �Ĵ�����ַ
 *		     data ���������ݽ�Ҫ��ŵĵ�ַ
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
