#ifndef __INIT_H
#define __INIT_H
#include "stm32f10x.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include  <stdio.h>
#include  <string.h>
#include "usart1.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "init.h"
#include "time.h"

extern float Yaw_y,Roll_y,Pitch_y;
void diantiao_init(void);
extern u8 sys_state;




#endif




