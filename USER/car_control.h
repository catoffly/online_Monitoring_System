#ifndef _CAR_CONTROL_H_
#define _CAR_CONTROL_H_
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "time.h"
#include "test.h"
#include  <stdio.h>
#include  <string.h>
#include "usart1.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "init.h"
#include "key_16.h"
#include "matrix.h"


void go_straight( u16 pwm_1 , u16 pwm_2 );
void turn_to_right( u16 pwm_1 , u16 pwm_2,u16 yanshi );
void turn_to_left( u16 pwm_1 , u16 pwm_2,u16 yanshi );
extern u16 yushe_cishu;


#endif
