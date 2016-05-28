#include "quaternion.h"
#include "other.h"
#include <math.h>

void quaternion_normalize(quaternion * q);
void quaternion_mult(quaternion * result,const quaternion * left,const quaternion * right);
void quaternion_rotateVector(const quaternion * rotation,const float from[3],float to[3]);
void quaternion_fromTwoVectorRotation(quaternion * result,const float from[3],const float to[3]);
void quaternion_fromFourVectorRotation(quaternion * result,const float from1[3],
    const float to1[3],const float from2[3],const float to2[3]);

void quaternion_normalize(quaternion * q)
{
    float norm_r = math_rsqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w *= norm_r;
    q->x *= norm_r;
    q->y *= norm_r;
    q->z *= norm_r;
}

// 四元数相乘
// left   : 被乘数，输入。
// right  : 乘数，输入。
// result : 积，输出。
void quaternion_mult(quaternion * result,const quaternion * left,const quaternion * right)
{
    result->w = left->w * right->w - left->x * right->x - left->y * right->y - left->z * right->z;
    result->x = left->x * right->w + left->w * right->x + left->y * right->z - left->z * right->y;
    result->y = left->y * right->w + left->w * right->y + left->z * right->x - left->x * right->z;
    result->z = left->z * right->w + left->w * right->z + left->x * right->y - left->y * right->x;
}

// 用四元数来旋转向量。
void quaternion_rotateVector(const quaternion * rotation,const float from[3],float to[3])
{
    float x2  = rotation->x * 2;
    float y2  = rotation->y * 2;
    float z2  = rotation->z * 2;
    float wx2 = rotation->w * x2;
    float wy2 = rotation->w * y2;
    float wz2 = rotation->w * z2;
    float xx2 = rotation->x * x2;
    float yy2 = rotation->y * y2;
    float zz2 = rotation->z * z2;
    float xy2 = rotation->x * y2;
    float yz2 = rotation->y * z2;
    float xz2 = rotation->z * x2;
    //
    to[0] = from[0]*(1 - yy2 - zz2) + from[1]*(xy2 - wz2)     + from[2]*(xz2 + wy2);
    to[1] = from[0]*(xy2 + wz2)     + from[1]*(1 - xx2 - zz2) + from[2]*(yz2 - wx2);
    to[2] = from[0]*(xz2 - wy2)     + from[1]*(yz2 + wx2)     + from[2]*(1 - xx2 - yy2);
}

//
// 两向量旋转→四元数旋转。
// 输入：from、to两向量，长度都必须大于0。
// 输出：从from方向转到to方向的旋转。
void quaternion_fromTwoVectorRotation(quaternion * result,const float from[3],const float to[3])
{
    float from_norm = fabsf(from[0]*from[0] + from[1]*from[1] + from[2]*from[2]);
    float to_norm = fabsf(to[0]*to[0] + to[1]*to[1] + to[2]*to[2]);
    //
    from_norm = sqrtf(from_norm);
    to_norm = sqrtf(to_norm);
    float cos_theta = (from[0]*to[0] + from[1]*to[1] + from[2]*to[2]) / (from_norm*to_norm);
    result->w = sqrtf((1.0f + cos_theta) / 2); // cos(theta/2)
    float sin_half_theta = sqrtf((1 - cos_theta) / 2);
    float cross_x = from[1]*to[2] - from[2]*to[1];
    float cross_y = from[2]*to[0] - from[0]*to[2];
    float cross_z = from[0]*to[1] - from[1]*to[0];
    if(cos_theta < 0)
    {
        cross_x = - cross_x;
        cross_y = - cross_y;
        cross_z = - cross_z;
    }
    float sin_half_theta_div_cross_norm = sin_half_theta /
        sqrtf(cross_x*cross_x + cross_y*cross_y + cross_z*cross_z);
    result->x = cross_x * sin_half_theta_div_cross_norm;
    result->y = cross_y * sin_half_theta_div_cross_norm;
    result->z = cross_z * sin_half_theta_div_cross_norm;
}

/*
 * 四向量旋转。
 * {from1,from2,to1,to2}都是单位向量。
 * 生成从{from1,from2}到{to1,to2}最接近的旋转。 */
void quaternion_fromFourVectorRotation(quaternion * result,const float from1[3],
    const float to1[3],const float from2[3],const float to2[3])
{
    /*
     * 变换基底。 */
    float mid_from[3],mid_to[3],cross_from[3],cross_to[3];
    math_vector_cross(cross_from,from1,from2);
    math_vector_cross(cross_to,to1,to2);
    for(int i=0;i<3;i++)
    {
        mid_from[i] = from1[i] + from2[i];
        mid_to[i] = to1[i] + to2[i];
    }
    /*
     * 先把mid转到重合。 */
    quaternion rotation_1;
    quaternion_fromTwoVectorRotation(&rotation_1,mid_from,mid_to);
    /*
     * 然后再把cross转到重合。 */
    quaternion rotation_2;
    float cross_from_1[3];
    quaternion_rotateVector(&rotation_1,cross_from,cross_from_1);
    quaternion_fromTwoVectorRotation(&rotation_2,cross_from_1,cross_to);
    /*
     * 最后结合两次旋转。 */
    quaternion_mult(result,&rotation_2,&rotation_1);
}
