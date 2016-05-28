#ifndef _ATTITUDE_ATTITUDE_H_
#define _ATTITUDE_ATTITUDE_H_

#include "time.h"
#include "calc.h"
#include "sensor.h"

extern  quaternion attitude;

void attitude_mixGyrAccMag(void);
void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval);

#endif
