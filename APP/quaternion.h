//     Copyright (c) 2013 js200300953@qq.com All rights reserved.
//         ==================================================
//         ========圆点博士微型四轴飞行器配套软件声明========
//         ==================================================
//     圆点博士微型四轴飞行器配套软件包括上位机程序、下位机Bootloader
// 、下位机App和遥控程序，及它们的源代码，以下总称“软件”。
//     软件仅提供参考，js200300953不对软件作任何担保，不对因使用该软件
// 而出现的损失负责。
//     软件可以以学习为目的修改和使用，但不允许以商业的目的使用该软件。
//     修改该软件时，必须保留原版权声明。
// 
//     更多资料见：
// http://blog.sina.com.cn/js200300953
// http://www.etootle.com/
// http://www.eeboard.com/bbs/forum-98-1.html#separatorline
// 圆点博士微型四轴飞行器QQ群：276721324

/* math/quaternion.h
 * 2012-11-7 18:26:09
 * js200300953
 */

#ifndef __FILE_MATH_QUATERNION_H__
#define __FILE_MATH_QUATERNION_H__

typedef  struct
{
    float w;
    float x;
    float y;
    float z;
}quaternion;

static inline void quaternion_loadIdentity(quaternion * q);
void quaternion_normalize(quaternion * q);
void quaternion_mult(quaternion * result,const quaternion * left,const quaternion * right);
void quaternion_rotateVector(const quaternion * rotation,const float from[3],float to[3]);
void quaternion_fromTwoVectorRotation(quaternion * result,const float from[3],const float to[3]);
void quaternion_fromFourVectorRotation(quaternion * result,const float from1[3],
    const float to1[3],const float from2[3],const float to2[3]);

static inline void quaternion_loadIdentity(quaternion * q)
{
    q->w = 1;
    q->x = q->y = q->z = 0;
}

#endif // __FILE_MATH_QUATERNION_H__
