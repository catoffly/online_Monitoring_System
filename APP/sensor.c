#include "sensor.h"
#include "calc.h"


int16_t ax, ay, az, gx, gy, gz;
float acc[3],gyo[3];
int16_t magx,magy,magz;

void sensor_init(void)
{
    Sen_Param_t_init();
	MPU6050_initialize();
	Delay_ms(300); //等待传感器稳定
	MPU6050_InitGyro_Offset();
	HMC5883L_SetUp();
	attitude_init();
}


void Data_Collect(void)
{
	MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	mpu6050_getCalibratedAcc(acc);
	mpu6050_getCalibratedGyr(gyo); 
	HMC58X3_getValues(&magx,&magy,&magz);
}


