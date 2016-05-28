#ifndef  _CALC_H_
#define  _CALC_H_

#include <stdint.h>
#include <math.h>


typedef  struct
{
    float w;
    float x;
    float y;
    float z;
} quaternion;

typedef struct
{
    float acc_gain[3];
    float gyr_gain[3];
    float mag_gain[3];

    //四元数
   quaternion horizon;
} Sen_Param_t;

static __INLINE  void quaternion_loadIdentity(quaternion * q)
{
    q->w = 1;
    q->x = q->y = q->z = 0;
}

extern Sen_Param_t param;

//外部函数申明
void Sen_Param_t_init(void);
Sen_Param_t* Get_Sen_Param_t(void);

void attitude_init(void);
float math_rsqrt(float number);
void quaternion_normalize(quaternion * q);

#endif
