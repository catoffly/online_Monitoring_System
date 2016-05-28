#ifndef __KEY_16_H_
#define __KEY_16_H_	 

#include "stm32f10x.h"

typedef struct  
{										    
   	u8 keytime;			//按键按下的次数
  	u8 keynum;			//按键的值

}_key_scan; 	  

extern _key_scan keyscan;	//管理按键的重要参数

//extern u8 k;  //记下按键的次数



void KEY_16_Init(void);//IO初始化
void KEY_16_Scan(void);  	//按键扫描函数
u8 KEY_getnum(void);	  //获取键盘录入的值
				    
#endif
