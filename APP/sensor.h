#ifndef  _SENSOR_H_
#define _SENSOR_H_

#include "mpu6050.h"
#include "HMC5883L.h"

extern float acc[3],gyo[3];

void sensor_init(void);
void Data_Collect(void);

#endif
