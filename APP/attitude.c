#include "attitude.h"
quaternion attitude;

#define PI 3.141592
static uint64_t time_pre_us = 0;


void attitude_init(void)
{
   quaternion_loadIdentity(&attitude);
}

void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval);

void attitude_mixGyrAccMag(void)
{
    uint64_t time_now_us = time_nowMs();
    int32_t time_interval_us = time_now_us - time_pre_us;
    time_pre_us = time_now_us;
    if(time_interval_us > 1000*1000) 
        return;
    //float time_interval_s = time_interval_us * (1.0f/1e6f);
	float time_interval_s = 10000 * (1.0f/1e6f);
   
    mix_gyrAcc_crossMethod(&attitude,acc,gyo,time_interval_s);
}

// 叉积法融合陀螺和加速度。
float Pitch[3]={0,0,0};

void mix_gyrAcc_crossMethod(quaternion * attitude,const float gyr[3],const float acc[3],float interval)
{
	const static float FACTOR = 0.001;
//	for(int i =0;i<3;i++)
//	{
		//Pitch[i] = Pitch[i]+ interval*gyr[i];	
	
//	}
    //
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    // 加速度计的读数，单位化。
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_aa = acc[0] * a_rsqrt;
    float y_aa = acc[1] * a_rsqrt;
    float z_aa = acc[2] * a_rsqrt;
    //
    // 载体坐标下的重力加速度常量，单位化。
    float x_ac = x_q*z_q_2 - w_q*y_q_2;
    float y_ac = y_q*z_q_2 + w_q*x_q_2;
    float z_ac = 1 - x_q*x_q_2 - y_q*y_q_2;
    //
    // 测量值与常量的叉积。
    float x_ca = y_aa * z_ac - z_aa * y_ac;
    float y_ca = z_aa * x_ac - x_aa * z_ac;
    float z_ca = x_aa * y_ac - y_aa * x_ac;
    //
    // 构造增量旋转。
    float delta_x = gyr[0] * interval / 2 + x_ca * FACTOR;
    float delta_y = gyr[1] * interval / 2 + y_ca * FACTOR;
    float delta_z = gyr[2] * interval / 2 + z_ca * FACTOR;
    //
    // 融合，四元数乘法。
    attitude->w = w_q         - x_q*delta_x - y_q*delta_y - z_q*delta_z;
    attitude->x = w_q*delta_x + x_q         + y_q*delta_z - z_q*delta_y;
    attitude->y = w_q*delta_y - x_q*delta_z + y_q         + z_q*delta_x;
    attitude->z = w_q*delta_z + x_q*delta_y - y_q*delta_x + z_q;
    quaternion_normalize(attitude);

	Pitch[0]= asin(-2 * attitude->x * attitude->z + 2 * attitude->w* attitude->y)* 57.3; // pitch

    Pitch[1] = atan2(2 * attitude->y * attitude->z + 2 * attitude->w * attitude->x, -2 * attitude->x * attitude->x - 2 * attitude->y* attitude->y + 1)* 57.3; // rollv
}  
