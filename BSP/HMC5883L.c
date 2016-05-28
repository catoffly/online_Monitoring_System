/******************** (C) COPYRIGHT 2013 YunMiao ********************
 * File Name          : main.c
 * Author             : YunMiao
 *Version            : V2.0.1
 * Date               : 08/01/20013
 * Description        : IIC basic function
 ********************************************************************************
 ********************************************************************************
 *******************************aircraft****************************************/

#include "HMC5883L.h"

/******************************************************************************
 *                                 �ڲ���������
 ******************************************************************************/

float x_scale, y_scale, z_scale, x_max, y_max, z_max;
float HMC5883_lastx, HMC5883_lasty, HMC5883_lastz;

int16_t HMC5883_FIFO[3][11]; //�������˲�
int16_t HMC5883_maxx = 0, HMC5883_maxy = 0, HMC5883_maxz = 0, HMC5883_minx =  - 0, HMC5883_miny =  - 0, HMC5883_minz =  - 0;
unsigned char HMC5883_calib = 0; //��ʼ����ɱ�־

void HMC58X3_getRaw(int16_t * x, int16_t * y, int16_t * z);


/******************************************************************************
 *                                 �ڲ���������
 ******************************************************************************/

/**************************ʵ�ֺ���********************************************
 *����ԭ��:	   unsigned char HMC5883_IS_newdata(void)
 *��������:	   ��ȡDRDY ���ţ��ж��Ƿ������һ��ת��
Low for 250 ��sec when data is placed in the data output registers.
���������  ��
���������  ������ת���������1  ������� 0
 *******************************************************************************/
unsigned char HMC5883_IS_newdata(void)
{
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == Bit_SET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/*****************************************************************************
 *��������:	HMC58X3_FIFO_init
 *��������:	������ȡ100�����ݣ��Գ�ʼ��FIFO����
 *��ڲ����� ��
 *��������� ��
 *******************************************************************************/
void HMC58X3_FIFO_init(void)
{
  int16_t temp[3];
  unsigned char i;
  for(i = 0; i < 50; i++)
  {
    HMC58X3_getRaw(&temp[0], &temp[1], &temp[2]);
    Delay_us(200); //��ʱ�ٶ�ȡ����
  }
}

/******************************************************************************
 *��������:	HMC58X3_newValues
 *��������:	����һ�����ݵ�FIFO����
 *��������� �������������Ӧ��ADCֵ
 *��������� ��
 *******************************************************************************/
void HMC58X3_newValues(int16_t x, int16_t y, int16_t z)
{
  unsigned char i;
  int32_t sum = 0;

  for(i = 1; i < 10; i++)
  {
    HMC5883_FIFO[0][i - 1] = HMC5883_FIFO[0][i];
    HMC5883_FIFO[1][i - 1] = HMC5883_FIFO[1][i];
    HMC5883_FIFO[2][i - 1] = HMC5883_FIFO[2][i];
  }
  HMC5883_FIFO[0][9] = x;
  HMC5883_FIFO[1][9] = y;
  HMC5883_FIFO[2][9] = z;

  sum = 0;
  for(i = 0; i < 10; i++)
  {
    //ȡ�����ڵ�ֵ���������ȡƽ��
    sum += HMC5883_FIFO[0][i];
  }
  HMC5883_FIFO[0][10] = sum / 10; //��ƽ��ֵ����

  sum = 0;
  for(i = 0; i < 10; i++)
  {
    sum += HMC5883_FIFO[1][i];
  }
  HMC5883_FIFO[1][10] = sum / 10;

  sum = 0;
  for(i = 0; i < 10; i++)
  {
    sum += HMC5883_FIFO[2][i];
  }
  HMC5883_FIFO[2][10] = sum / 10;

  if(HMC5883_calib)
  {
    //У��
    if(HMC5883_minx > HMC5883_FIFO[0][10])
    {
      HMC5883_minx = HMC5883_FIFO[0][10];
    }
    if(HMC5883_miny > HMC5883_FIFO[1][10])
    {
      HMC5883_miny = HMC5883_FIFO[1][10];
    }
    if(HMC5883_minz > HMC5883_FIFO[2][10])
    {
      HMC5883_minz = HMC5883_FIFO[2][10];
    }

    if(HMC5883_maxx < HMC5883_FIFO[0][10])
    {
      HMC5883_maxx = HMC5883_FIFO[0][10];
    }
    if(HMC5883_maxy < HMC5883_FIFO[1][10])
    {
      HMC5883_maxy = HMC5883_FIFO[1][10];
    }
    if(HMC5883_maxz < HMC5883_FIFO[2][10])
    {
      HMC5883_maxz = HMC5883_FIFO[2][10];
    }
  }

}

/******************************************************************************
 *��������:	 HMC58X3_writeReg
 *��������:	 дHMC5883L�ļĴ���
 *��ڲ�����  reg  �Ĵ�����ַ
 *			 val  Ҫд���ֵ	
 *���������  ��
 *******************************************************************************/
void HMC58X3_writeReg(unsigned char reg, unsigned char val)
{
  IICwriteByte(HMC58X3_ADDR, reg, val);
}

/******************************************************************************
 *��������:	 HMC58X3_getRaw
 *��������:	 ��ȡ��ǰ�Ĵ���ֵ
 *��ڲ�����  �������Ӧ�����ָ��			 	
 *���������  ��
 *******************************************************************************/
void HMC58X3_getRaw(int16_t * x, int16_t * y, int16_t * z)
{
  unsigned char vbuff[6];
  vbuff[0] = vbuff[1] = vbuff[2] = vbuff[3] = vbuff[4] = vbuff[5] = 0;
  IICreadBytes(HMC58X3_ADDR, HMC58X3_R_XM, 6, vbuff);
  HMC58X3_newValues(((int16_t)vbuff[0] << 8) | vbuff[1], ((int16_t)vbuff[4] << 8) | vbuff[5], ((int16_t)vbuff[2] << 8) | vbuff[3]);
  *x = HMC5883_FIFO[0][10];
  *y = HMC5883_FIFO[1][10];
  *z = HMC5883_FIFO[2][10];
}

/******************************************************************************
 *��������:	HMC58X3_getValues
 *��������:	��ȡ �����Ƶĵ�ǰADCֵ
 *��ڲ����� �������Ӧ�����ָ��	
 *��������� ��
 *******************************************************************************/
void HMC58X3_getValues(int16_t * x, int16_t * y, int16_t * z)
{
  int16_t xr, yr, zr;
  HMC58X3_getRaw(&xr, &yr, &zr);
  *x = (xr); 
  *y = (yr); 
  *z = (zr); 
}

/******************************************************************************
 *��������:	HMC58X3_getlastValues
 *��������:	��ȡ �����Ƶĵ�ǰADCֵ
 *��������� �������Ӧ�����ָ��	
 *��������� ��
 *******************************************************************************/
void HMC58X3_getlastValues(int16_t * x, int16_t * y, int16_t * z)
{
  *x = HMC5883_FIFO[0][10]; 
  *y = HMC5883_FIFO[1][10]; 
  *z = HMC5883_FIFO[2][10]; 
}

/******************************************************************************
 *��������:	 HMC58X3_mgetValues
 *��������:	 ��ȡ У����� ������ADCֵ
 *���������  �������ָ��	
 *���������  ��
 *******************************************************************************/
void HMC58X3_mgetValues(float * arry)
{
  int16_t xr, yr, zr;
  HMC58X3_getRaw(&xr, &yr, &zr);
  arry[0] = HMC5883_lastx = (float)(xr - ((HMC5883_maxx + HMC5883_minx) / 2));
  arry[1] = HMC5883_lasty = (float)(yr - ((HMC5883_maxy + HMC5883_miny) / 2));
  arry[2] = HMC5883_lastz = (float)(zr - ((HMC5883_maxz + HMC5883_minz) / 2));
}

/******************************************************************************
 *��������:	HMC58X3_setGain
 *��������:	���� 5883L������
 *��������� Ŀ������ 0-7
 *��������� ��
 *******************************************************************************/
void HMC58X3_setGain(unsigned char gain)
{
  // 0-7, 1 default
  if(gain > 7)
  {
    return ;
  }
  HMC58X3_writeReg(HMC58X3_R_CONFB, gain << 5);
}

/******************************************************************************
 *��������:	HMC58X3_setMode
 *��������:	���� 5883L�Ĺ���ģʽ
 *��������� ģʽ
 *��������� ��
 *******************************************************************************/
void HMC58X3_setMode(unsigned char mode)
{
  if(mode > 2)
  {
    return ;
  }
  HMC58X3_writeReg(HMC58X3_R_MODE, mode);
  Delay_us(100);
}

/*****************************************************************************
 *��������:	  HMC58X3_init
 *��������:	  ���� 5883L�Ĺ���ģʽ
 *���������   ģʽ
 *���������   ��
 *******************************************************************************/
void HMC58X3_init(u8 setmode)
{
  // note that we don't initialize Wire here.
  Delay_us(5); // you need to wait at least 5ms after power on to initialize
  if(setmode)
  {
    HMC58X3_setMode(0);
  }
  x_scale = 1.0; // get actual values
  y_scale = 1.0;
  z_scale = 1.0;

  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x70); // 8 samples averaged, 75Hz frequency, no artificial bias.
  HMC58X3_writeReg(HMC58X3_R_CONFB, 0xA0);
  HMC58X3_writeReg(HMC58X3_R_MODE, 0x00);

}

/*
Calibrate which has a few weaknesses.
1. Uses wrong gain for first reading.
2. Uses max instead of max of average when normalizing the axis to one another.
3. Doesn't use neg bias. (possible improvement in measurement).
 */
void HMC58X3_calibrate(unsigned char gain)
{
  int i;
  int16_t x, y, z;
  float mx =  - 200000.0, my =  - 200000.0, mz =  - 200000.0;
  float max = 0;
  x_scale = 1.0; // get actual values
  y_scale = 1.0;
  z_scale = 1.0;

  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x010 + HMC_POS_BIAS); // Reg A DOR=0x010 + MS1,MS0 set to pos bias
  HMC58X3_setGain(gain);
  HMC58X3_setMode(1);

  for(i = 0; i < 10; i++)
  {
    //while((HMC5883_IS_newdata() == 0))
     // ;
    HMC58X3_getValues(&x, &y, &z);
    if(x > mx)
    {
      mx = (float)x;
    }
    if(y > my)
    {
      my = (float)y;
    }
    if(z > mz)
    {
      mz = (float)z;
    }
  }

  if(mx > max)
  {
    max = mx;
  }
  if(my > max)
  {
    max = my;
  }
  if(mz > max)
  {
    max = mz;
  }
  x_max = mx;
  y_max = my;
  z_max = mz;
  x_scale = max / mx; // calc scales
  y_scale = max / my;
  z_scale = max / mz;

  HMC58X3_writeReg(HMC58X3_R_CONFA, 0x010); // set RegA/DOR back to default
} // calibrate().

/******************************************************************************
 *��������:	      HMC58X3_setDOR
 *��������:	      ���� 5883L�� �����������
 *��ڲ�����       ����ֵ
 *0 -> 0.75Hz  |   1 -> 1.5Hz
 *2 -> 3Hz     |   3 -> 7.5Hz
 *4 -> 15Hz    |   5 -> 30Hz
 *6 -> 75Hz
 *���������  ��
 *******************************************************************************/
void HMC58X3_setDOR(unsigned char DOR)
{
  if(DOR > 6)
  {
    return ;
  }
  HMC58X3_writeReg(HMC58X3_R_CONFA, DOR << 2);
}

/*******************************************************************************
 *��������:	 HMC58X3_getID
 *��������:	 ��ȡоƬ��ID
 *��ڲ�����  ID��ŵ�����
 *���������  ��
 *******************************************************************************/
void HMC58X3_getID(char id[3])
{
  id[0] = I2C_ReadOneByte(HMC58X3_ADDR, HMC58X3_R_IDA);
  id[1] = I2C_ReadOneByte(HMC58X3_ADDR, HMC58X3_R_IDB);
  id[2] = I2C_ReadOneByte(HMC58X3_ADDR, HMC58X3_R_IDC);
} // getID().

/******************************************************************************
 *��������:	 HMC5883L_SetUp
 *��������:	 ��ʼ�� HMC5883L ʹ֮�������״̬
 *���������  ��  	
 *���������  ��
 *******************************************************************************/
void HMC5883L_SetUp(void)
{
  HMC5883_calib = 0;
  HMC58X3_init(0); // Don't set mode yet, we'll do that later on.
  // Calibrate HMC using self test, not recommended to change the gain after calibration.
  HMC58X3_calibrate(1); // Use gain 1=default, valid 0-7, 7 not recommended.
  // Single mode conversion was used in calibration, now set continuous mode
  HMC58X3_setMode(0);
  Delay_us(100);
  HMC58X3_setDOR(6); //75hz
  Delay_us(50);
  HMC58X3_FIFO_init();

  Read_Magic_Offset(&HMC5883_minx, &HMC5883_miny, &HMC5883_minz,  &HMC5883_maxx, &HMC5883_maxy, &HMC5883_maxz);


}

/******************************************************************************
 *��������:	 HMC5883L_Start_Calib
 *��������:	 ��������Ʊ궨
 *���������  ��  	
 *���������  ��
 *******************************************************************************/
void HMC5883L_Start_Calib(void)
{
  HMC5883_calib = 1; //��ʼ�궨
  HMC5883_maxx = 0;
  HMC5883_maxy = 0;
  HMC5883_maxz = 0;
  HMC5883_minx =  - 0;
  HMC5883_miny =  - 0;
  HMC5883_minz =  - 0;
}

/******************************************************************************
 *��������:	  void HMC5883L_Save_Calib(void)
 *��������:	  ��������Ʊ궨ֵ ��Flash
 *���������   ��  	
 *���������  ��
 *******************************************************************************/
void HMC5883L_Save_Calib(void)
{

  Write_Magic_Offset(HMC5883_minx, HMC5883_miny, HMC5883_minz, HMC5883_maxx, HMC5883_maxy, HMC5883_maxz);

  HMC5883_calib = 0; //�����궨

  Read_Magic_Offset(&HMC5883_minx, &HMC5883_miny, &HMC5883_minz,  &HMC5883_maxx, &HMC5883_maxy, &HMC5883_maxz);
}
