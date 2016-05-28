#include "eeprom.h"

#define  PAGE_Gyro   (0x08000000 + 60 * 1024) //�������ǵ�ƫ�ô���ڵ�60ҳFlash
#define  PAGE_Magic  (0x08000000 + 61 * 1024) //�������Ƶı궨ֵ����ڵ�61ҳFlash



/*******************************************************************************
*��������:	 Read_Gyro_Offset
*��������:	 ��ȡ�����Flash�е�������ƫ�����ݡ�
*��ڲ�����  �������ƫ�ã����ָ��
*���������  ��
*******************************************************************************/
void Read_Gyro_Offset(int16_t *offset_gx,int16_t *offset_gy,int16_t *offset_gz)                                
{
    int16_t *temp_addr = (int16_t *)PAGE_Gyro;
	*offset_gx=*temp_addr;
	temp_addr++;
    *offset_gy=*temp_addr;
	temp_addr++;
    *offset_gz=*temp_addr;                                                                                             
}

/******************************************************************************
*��������:	 Write_Gyro_Offset
*��������:	 ��������ƫ�ô�ŵ�Flash
*��ڲ�����  �������ƫ��
*���������  ��
*******************************************************************************/
void Write_Gyro_Offset(int16_t offset_gx,int16_t offset_gy,int16_t offset_gz)
{
 FLASH_Status temp_stat;
 uint32_t temp_addr = PAGE_Gyro;
 FLASH_Unlock();
 temp_stat = FLASH_ErasePage(PAGE_Gyro); 
 if(temp_stat != FLASH_COMPLETE)
        {
          FLASH_Lock();
          return ;
        }

temp_stat = FLASH_ProgramHalfWord(temp_addr,offset_gx);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,offset_gy);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,offset_gz);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

FLASH_Lock();    
}

/********************************************************************************
*��������:	 Read_Magic_Offset)
*��������:	 ��Flash��ȡ�����Ʊ궨
*��ڲ�����  ������������Ķ�Ӧ�����ֵ����Сֵ
*���������  ��
*******************************************************************************/
void Read_Magic_Offset(int16_t *min_mx,int16_t *min_my,int16_t *min_mz,
int16_t *max_mx,int16_t *max_my,int16_t *max_mz)                                
{
    int16_t *temp_addr = (int16_t *)PAGE_Magic;
	*min_mx=*temp_addr;
	temp_addr++;
    *min_my=*temp_addr;
	temp_addr++;
    *min_mz=*temp_addr;   
	temp_addr++;

	*max_mx=*temp_addr;   
	temp_addr++;
	*max_my=*temp_addr;   
	temp_addr++;
	*max_mz=*temp_addr;                                                                  
}

/*******************************************************************************
*��������:	Write_Magic_Offset  
*��������:	�������Ʊ궨д��Flash
*��������� ������������Ķ�Ӧ�����ֵ����Сֵ
*��������� ��
*******************************************************************************/
void Write_Magic_Offset(int16_t min_mx,int16_t min_my,int16_t min_mz,
int16_t max_mx,int16_t max_my,int16_t max_mz)
{
 FLASH_Status temp_stat;
 uint32_t temp_addr = PAGE_Magic;
 FLASH_Unlock();
 temp_stat = FLASH_ErasePage(PAGE_Gyro); 
 if(temp_stat != FLASH_COMPLETE)
        {
          FLASH_Lock();
          return ;
        }

temp_stat = FLASH_ProgramHalfWord(temp_addr,min_mx);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,min_my);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,min_mz);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,max_mx);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,max_my);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}
temp_addr+=2;

temp_stat = FLASH_ProgramHalfWord(temp_addr,max_mz);
if(temp_stat != FLASH_COMPLETE)
{
    FLASH_Lock();
    return ;
}

FLASH_Lock();    
}

