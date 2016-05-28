#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"


void Read_Gyro_Offset(int16_t *offset_gx,int16_t *offset_gy,int16_t *offset_gz);
void Write_Gyro_Offset(int16_t offset_gx,int16_t offset_gy,int16_t offset_gz);
void Read_Magic_Offset(int16_t *min_mx,int16_t *min_my,int16_t *min_mz,
int16_t *max_mx,int16_t *max_my,int16_t *max_mz);
void Write_Magic_Offset(int16_t min_mx,int16_t min_my,int16_t min_mz,
int16_t max_mx,int16_t max_my,int16_t max_mz);

#endif /* __EEPROM_H */

//------------------End of File----------------------------
