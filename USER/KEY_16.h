#ifndef __KEY_16_H_
#define __KEY_16_H_	 

#include "stm32f10x.h"

typedef struct  
{										    
   	u8 keytime;			//�������µĴ���
  	u8 keynum;			//������ֵ

}_key_scan; 	  

extern _key_scan keyscan;	//����������Ҫ����

//extern u8 k;  //���°����Ĵ���



void KEY_16_Init(void);//IO��ʼ��
void KEY_16_Scan(void);  	//����ɨ�躯��
u8 KEY_getnum(void);	  //��ȡ����¼���ֵ
				    
#endif
