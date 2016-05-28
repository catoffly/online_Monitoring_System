#include "calc.h"

Sen_Param_t param;

void Sen_Param_t_Defauinit(Sen_Param_t* param)
{
	 for(uint8_t i=0;i<3;i++)
    {
        param->acc_gain[i]   = 9.7883 / 4096.0;
        param->gyr_gain[i]   = (3.1415926/180.0) /16.4;
        param->mag_gain[i]   = 1;
    }
   //世界坐标原点
    param->horizon.w = 1;
    param->horizon.x = 0;
    param->horizon.y = 0;
    param->horizon.z = 0;

}

void Sen_Param_t_init(void)
{
	Sen_Param_t_Defauinit(&param);
}


Sen_Param_t* Get_Sen_Param_t(void)
{
	return &param;
}

/******************************************************************************
*函数名称:	math_rsqrt
*函数功能:	快速计算平方根倒数
*入口参数   计算的数
*******************************************************************************/
float math_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       
    i  = 0x5f3759df - ( i >> 1 );               
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   
    y  = y * ( threehalfs - ( x2 * y * y ) );  

    return y;
}

/******************************************************************************
*函数名称:	quaternion_normalize
*函数功能:	四元数归一
*入口参数   四元数
*******************************************************************************/ 
void quaternion_normalize(quaternion * q)
{
    float norm_r = math_rsqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w *= norm_r;
    q->x *= norm_r;
    q->y *= norm_r;
    q->z *= norm_r;
}
